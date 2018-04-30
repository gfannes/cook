#ifndef HEADER_cook_process_command_msvc_Compiler_hpp_ALREADY_INCLUDED
#define HEADER_cook_process_command_msvc_Compiler_hpp_ALREADY_INCLUDED

#include "cook/process/command/Compiler.hpp"
#include "cook/Language.hpp"
#include <string>

namespace cook { namespace process { namespace command { namespace msvc { 

    class Compiler : public command::Compiler
    {
    public:
        Compiler(Language);

        std::string name() const override;
        Result process(const std::list<std::filesystem::path> & input_files, const std::list<std::filesystem::path> & output_files) override;
        void to_stream(std::ostream & oss, const std::list<std::filesystem::path> & input_files, const std::list<std::filesystem::path> & output_files) override;

        void add_define(const std::string & name, const std::string & value) override;
        void add_define(const std::string & name) override;
        void add_include_path(const std::filesystem::path & path) override;
        void add_force_include(const std::filesystem::path & path) override;

    private:
        ArgumentDesc input_;
        ArgumentDesc output_;
        ArgumentDesc define_;
        ArgumentDesc include_;
        ArgumentDesc force_include_;
    };

} } } } 

#endif