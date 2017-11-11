#ifndef HEADER_cook_presenter_NinjaWriter_hpp_ALREADY_INCLUDED
#define HEADER_cook_presenter_NinjaWriter_hpp_ALREADY_INCLUDED

#include "cook/model/toolchain/Toolchain.hpp"
#include "gubg/std/filesystem.hpp"
#include "gubg/string_algo/substitute.hpp"
#include "gubg/mss.hpp"
#include <ostream>

namespace cook { namespace presenter { 

    class NinjaWriter
    {
    public:
        NinjaWriter(std::ostream &os): os_(os) { }

        bool write(const model::Env &env, const model::toolchain::Toolchain &toolchain, const model::RecipeDAG &dag)
        {
            MSS_BEGIN(bool);
            MSS(write_(env));
            MSS(write_(toolchain));
            MSS(write_(dag));
            MSS_END();
        }

    private:
        bool write_(const model::Env &env)
        {
            MSS_BEGIN(bool);
            os_ << std::endl;
            os_ << "# >> Environment" << std::endl;
            os_ << "builddir = " << env.build_dir().string() << std::endl;
            os_ << "# << Environment" << std::endl;
            MSS_END();
        }
        bool write_(const model::toolchain::Toolchain &toolchain)
        {
            MSS_BEGIN(bool);
            os_ << std::endl;
            os_ << "# >> Toolchain" << std::endl;
            model::toolchain::Compiler::Ptr compiler;
            model::toolchain::Linker::Ptr linker;
            model::toolchain::Archiver::Ptr archiver;
            MSS(toolchain.get_info(compiler, linker, archiver));

            //Compiler rules and global settings
            {
                model::toolchain::Compiler::TemplateStubs cstubs;
                {
                    cstubs.object = "$out";
                    cstubs.source = "$in";
                    cstubs.depfile = "$out.d";
                    cstubs.flags = "$cflags";
                    cstubs.defines = "$defines";
                    cstubs.include_paths = "$include_paths";
                    cstubs.force_includes = "$force_includes";
                }

                os_
                << "rule compile_c" << std::endl
                << "  command = " << compiler->cmd_template("c", cstubs) << std::endl
                << "  depfile = $out.d" << std::endl;

                os_
                << "rule compile_cpp" << std::endl
                << "  command = " << compiler->cmd_template("c++", cstubs) << std::endl
                << "  depfile = $out.d" << std::endl;

                os_
                << "rule compile_asm" << std::endl
                << "  command = " << compiler->cmd_template("asm", cstubs) << std::endl
                << "  depfile = $out.d" << std::endl;

                model::toolchain::Flags flags;
                os_ << "cflags = " << compiler->prepare_flags(flags) << std::endl;
                model::toolchain::Defines defines;
                os_ << "defines = " << compiler->prepare_defines(defines) << std::endl;

                os_ << std::endl;
            }

            //Linker rules and global settings
            {
                model::toolchain::Linker::TemplateStubs lstubs;
                {
                    lstubs.executable = "$out";
                    lstubs.objects = "$in";
                    lstubs.flags = "$lflags";
                }

                os_
                << "rule link" << std::endl
                << "  command = " << linker->cmd_template(lstubs) << std::endl;

                model::toolchain::Flags flags;
                os_ << "lflags = " << linker->prepare_flags(flags) << std::endl;

                os_ << std::endl;
            }

            //Archiver rules and global settings
            {
                model::toolchain::Archiver::TemplateStubs astubs;
                {
                    astubs.library = "$out";
                    astubs.objects = "$in";
                    astubs.flags = "$aflags";
                }

                os_
                << "rule archive" << std::endl
                << "  command = " << archiver->cmd_template(astubs) << std::endl;

                model::toolchain::Flags flags;
                os_ << "aflags = " << linker->prepare_flags(flags) << std::endl;

                os_ << std::endl;
            }

            os_ << "# << Toolchain" << std::endl;
            MSS_END();
        }
        static std::string escape_(const std::string &orig)
        {
            std::string esc;
            gubg::string_algo::substitute<std::string>(esc, orig, ":", "$:");
            return esc;
        }
        bool write_(const model::RecipeDAG &dag)
        {
            MSS_BEGIN(bool);
            std::ostringstream oss;
            auto object_fn = [&](const auto &file){
                oss.str("");
                oss << file.path.string() << ".obj";
                return escape_(oss.str());
            };
            auto compile_rule = [](const auto &file){
                if (false) {}
                else if (file.language == "c") return "compile_c";
                else if (file.language == "c++") return "compile_cpp";
                else if (file.language == "asm") return "compile_asm";
                return "";
            };
            auto source_fn = [](const auto &file){
                return escape_(file.path.string());
            };
            auto local_name = [&](const model::Recipe &r, const char *name){
                oss.str("");
                oss << r.uri().str('\0', '_', '_') << "_" << name;
                return oss.str();
            };
            auto write_recipe = [&](const model::Recipe &recipe){
                MSS_BEGIN(bool);
                os_ << std::endl;
                os_ << "# >> Recipe " << recipe.uri_hr() << std::endl;
                os_ << local_name(recipe, "include_paths") << " =";
                for (const auto &ip: recipe.include_paths())
                    os_ << " -I " << ip.string();
                os_ << std::endl;
                auto write_compile = [&](const auto &file){
                    if (file.type == model::FileType::Source)
                    {
                        const auto obj_fn = object_fn(file);
                        os_ << "build " << obj_fn << ": " << compile_rule(file) << " " << source_fn(file) << std::endl;
                        os_ << "    defines = " << std::endl;
                        os_ << "    include_paths = $" << local_name(recipe, "include_paths");
                        auto add_ip_for_deps = [&](const model::Recipe &d){
                            os_ << " $" << local_name(d, "include_paths");
                            return true;
                        };
                        dag.each_out(&recipe, add_ip_for_deps);
                        os_ << std::endl;
                        os_ << "    force_includes = " << std::endl;
                        os_ << "    library_paths = " << std::endl;
                        os_ << "    libraries = " << std::endl;
                    }
                    return true;
                };
                MSS(recipe.each_file(write_compile, model::Owner::Me));
                if (false) {}
                else if (recipe.type() == "executable")
                {
                    os_ << "build " << recipe.output().filename.string() << ": link ";
                    auto add_object = [&](const auto &f){
                        if (f.type == model::FileType::Source)
                            os_ << "$\n    " << object_fn(f) << " ";
                        return true;
                    };
                    MSS(recipe.each_file(add_object, model::Owner::Anybody));
                    os_ << std::endl;
                }
                os_ << "# << Recipe " << recipe.uri_hr() << std::endl;
                MSS_END();
            };
            dag.each_vertex<gubg::network::Direction::Backward>(write_recipe);
            MSS_END();
        }

        std::ostream &os_;
    };

} } 

#endif
