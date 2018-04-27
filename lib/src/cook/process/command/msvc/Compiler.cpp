#include "cook/process/command/msvc/Compiler.hpp"

namespace cook { namespace process { namespace command { namespace msvc { 

    Compiler::Compiler(Language language):
        input_(10),
        output_(20, "/Fo:"),
        define_(30, "-D"),
        include_(40, "-I"),
        force_include_(50, "/FI")
    {
        set_command("cl");
        switch (language)
        {
            case Language::CXX:
                break;
            case Language::C:
                add_argument(ArgumentDesc(0, "/TC"), std::string());
                break;

        default:
            break;
        }
        add_argument(ArgumentDesc(2, "-c"), std::string());
    }

    std::string Compiler::name() const {return "msvc";}

    Result Compiler::process(const std::list<std::filesystem::path> & input_files, const std::list<std::filesystem::path> & output_files)
    {
        MSS_BEGIN(Result);
        MSS_END();
    }
    void Compiler::to_stream(std::ostream & oss, const std::list<std::filesystem::path> & input_files, const std::list<std::filesystem::path> & output_files)
    {
        OrderedCommand::to_stream(oss, input_, input_files, output_, output_files);
    }

    void Compiler::add_define(const std::string & name, const std::string & value)
    {
        add_argument(define_, gubg::stream([&](auto & os) { os << name << "=" << value; }));
    }
    void Compiler::add_define(const std::string & name)
    {
        add_argument(define_, name);
    }
    void Compiler::add_include_path(const std::filesystem::path & path)
    {
        add_argument(include_, path.string());
    }
    void Compiler::add_force_include(const std::filesystem::path & path)
    {
        add_argument(force_include_, path.string());
    }

} } } } 
