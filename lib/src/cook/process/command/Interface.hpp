#ifndef HEADER_cook_process_command_Interface_hpp_ALREADY_INCLUDED
#define HEADER_cook_process_command_Interface_hpp_ALREADY_INCLUDED

#include "cook/Result.hpp"
#include "cook/process/toolchain/Types.hpp"
#include "cook/LanguageTypePair.hpp"
#include "cook/ingredient/File.hpp"
#include "cook/ingredient/KeyValue.hpp"
#include "gubg/std/filesystem.hpp"
#include <string>
#include <memory>

namespace cook { namespace process { namespace command {

    using Filename = std::filesystem::path;
    using Filenames = std::list<Filename>;

    class Interface
    {
    public:
        using PartEscapeFunctor = std::function<std::string (toolchain::Part, const std::string &)>;
        
        enum Type {
            Compile,
            Archive,
            Link,
            UserDefined
        };

        virtual ~Interface() {}

        virtual std::string name() const = 0;
        virtual Type type() const = 0;

        virtual void set_inputs_outputs(const Filenames & input_files, const Filenames & output_files) = 0;
        virtual bool stream_part(std::ostream &, toolchain::Part, const toolchain::Translator * = nullptr, const PartEscapeFunctor & functor = PartEscapeFunctor()) const = 0;
        virtual std::string get_kv_part(toolchain::Part part, const std::string & key, const std::string & value = std::string(), toolchain::Translator * trans_ptr = nullptr) const = 0;
        virtual void stream_command(std::ostream &, const PartEscapeFunctor & functor = PartEscapeFunctor()) const = 0;
        virtual Result process() = 0;

        virtual std::string recipe_uri() const  = 0;

        virtual bool process_ingredient(const LanguageTypePair& ltp, const ingredient::File& file)  = 0;
        virtual bool process_ingredient(const LanguageTypePair& ltp, const ingredient::KeyValue& key_value)  = 0;
        virtual bool delete_before_build() const = 0;
    };

    inline std::ostream & operator<<(std::ostream & str, Interface::Type type)
    {
        switch(type)
        {
#define L_CASE(TYPE, STR) case Interface::TYPE: return str << STR
            L_CASE(Compile, "compile");
            L_CASE(Archive, "archive");
            L_CASE(Link, "link");
#undef L_CASE
            default: break;
        }

        if (type >= Interface::UserDefined)
            return str << "userdefined_" << static_cast<unsigned int>(type);
        else
            return str << "unknown";

    }

    using Ptr = std::shared_ptr<Interface>;

} } }

#endif
