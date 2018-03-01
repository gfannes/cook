#ifndef HEADER_cook_process_souschef_Compiler_hpp_ALREADY_INCLUDED
#define HEADER_cook_process_souschef_Compiler_hpp_ALREADY_INCLUDED

#include "cook/process/souschef/Interface.hpp"
#include "cook/process/build/Command.hpp"
#include "gubg/stream.hpp"

namespace cook { namespace process { namespace souschef {

class Compiler : public Interface
{
public:
    explicit Compiler(Language language);

    std::string description() const override { return gubg::stream([&](auto & os) { os << language_ << " compiler"; }); }
    Result process(const Context & context, model::Snapshot & snapshot) const override;

private:
    virtual ingredient::File construct_object_file(const ingredient::File & source, const Context &context) const;
    virtual build::CommandPtr compile_command(const Context & context) const;


    Language language_;

};

} } }



#endif
