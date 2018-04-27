#include "cook/process/command/gcclike/Compiler.hpp"

namespace cook { namespace process { namespace command { namespace gcclike {

Compiler::Compiler(std::string cli_c, std::string cli_cxx, Language language)
    : input_(10),
      output_(20, "-o"),
      define_(30, "-D"),
      include_(40, "-I"),
      force_include_(50, "-include")
{
    switch (language)
    {
        case Language::CXX:
            set_command(cli_cxx);
            add_argument(ArgumentDesc(0, "-std=c++17"), std::string());
            break;
        case Language::C:
            set_command(cli_c);
            break;

    default:
        break;
    }
    add_argument(ArgumentDesc(1, "-g"), std::string());
    add_argument(ArgumentDesc(2, "-c"), std::string());
}

void Compiler::add_define(const std::string & name, const std::string & value)
{
    add_argument(define_, gubg::stream([&](auto & os) { os << name << "=" << value; }));
}
void Compiler::add_define(const std::string & name)
{
    add_argument(define_, name);
}

void Compiler::add_include_path(const std::filesystem::path &path)
{
    add_argument(include_, path.string());
}

void Compiler::add_force_include(const std::filesystem::path & path)
{
    add_argument(force_include_, path.string());
}

std::string Compiler::name() const
{
    return gubg::stream([&](auto & os) { os << "compiler [" << command()  << "]"; });
}

Result Compiler::process(const std::list<std::filesystem::path> & input_files, const std::list<std::filesystem::path> & output_files)
{
    return Result();
}

void Compiler::to_stream(std::ostream & oss, const std::list<std::filesystem::path> & input_files, const std::list<std::filesystem::path> & output_files)
{
    OrderedCommand::to_stream(oss, input_, input_files, output_, output_files);
}

} } } }
