#include "cook/chai/Loader.hpp"
#include "cook/chai/Details.hpp"
#include "cook/structure/Recipe.hpp"
#include "cook/util/TreeNode.hpp"
#include <functional>

namespace cook { namespace chai { 

bool Loader::load(structure::Book & root)
{
    MSS_BEGIN(bool);
    
    GlobalInfo info(options_, root);

    // initialize the engine
    auto & chai = info.engine.raw();
    chai.add(chaiscript::fun(&GlobalInfo::include, &info),              "include");
    chai.add(chaiscript::fun(&GlobalInfo::open_book_single, &info),     "open_book");
    chai.add(chaiscript::fun(&GlobalInfo::open_book_ar, &info),     "open_book");
    chai.add(chaiscript::fun(&GlobalInfo::create_new_recipe, &info),    "add_recipe");
    chai.add(chaiscript::fun(&GlobalInfo::set_display_name, &info),     "display_name");
    chai.add(chaiscript::fun(&GlobalInfo::print, &info),                "print");
    
    chai.add(chaiscript::fun(&BookWrapper::include),                    "include");
    chai.add(chaiscript::fun(&BookWrapper::open_book_single),           "open_book");
    chai.add(chaiscript::fun(&BookWrapper::open_book_ar),           "open_book");
    chai.add(chaiscript::fun(&BookWrapper::create_new_recipe),          "add_recipe");
    chai.add(chaiscript::fun(&BookWrapper::set_display_name),           "display_name");
    chai.add(chaiscript::fun(&BookWrapper::print),                      "print");
    
    chai.add(chaiscript::fun(&RecipeWrapper::add_source),               "add_source");
    chai.add(chaiscript::fun(&RecipeWrapper::add_header),               "add_header");
    chai.add(chaiscript::fun(&RecipeWrapper::add_include_path),         "add_include_path");
    chai.add(chaiscript::fun(&RecipeWrapper::add_force_include),        "add_force_include");
    chai.add(chaiscript::fun(&RecipeWrapper::add_define_1),             "add_define");
    chai.add(chaiscript::fun(&RecipeWrapper::add_define_2),             "add_define");
    chai.add(chaiscript::fun(&RecipeWrapper::add_library_path),         "add_library_path");
    chai.add(chaiscript::fun(&RecipeWrapper::add_library),              "add_library");
    chai.add(chaiscript::fun(&RecipeWrapper::depends_on),               "depends_on");
    chai.add(chaiscript::fun(&RecipeWrapper::print),                    "print");
    chai.add(chaiscript::fun(&RecipeWrapper::set_display_name),         "display_name");
    chai.add(chaiscript::fun(&RecipeWrapper::set_target_type),          "target_type");
    
    chai.add_global_const(chaiscript::const_var(structure::TargetType::Executable), "executable");
    chai.add_global_const(chaiscript::const_var(structure::TargetType::StaticLibrary), "static_library");

    try
    {
        info.include(root.script_filename());
    }
    catch (const chaiscript::exception::eval_error &exc)
    {
        if (!exc.call_stack.empty())
        {
//            std::cout << exc.detail << std::endl;

            auto & el = exc.call_stack[0];

            auto n = util::make_tree_node(std::cout, "error");
            n.attr("script", el.filename());
            n.attr("line", el.start().line);
            n.attr("column", el.start().column);
            n.attr("error", exc.what());
            n.attr("detail", exc.detail);
        }

        MSS(false);
    }
    
    MSS_END();
}

} }
