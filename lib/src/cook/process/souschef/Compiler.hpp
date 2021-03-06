#ifndef HEADER_cook_process_souschef_Compiler_hpp_ALREADY_INCLUDED
#define HEADER_cook_process_souschef_Compiler_hpp_ALREADY_INCLUDED

#include "cook/process/souschef/Interface.hpp"
#include "cook/process/command/Interface.hpp"
#include "cook/process/toolchain/Element.hpp"
#include "gubg/stream.hpp"

namespace cook { namespace process { namespace souschef {

class Compiler : public Interface
{
public:
    explicit Compiler(Language language);

    std::string description() const override { return gubg::stream([&](auto & os) { os << language_ << " compiler"; }); }
    Result process(model::Recipe & recipe, RecipeFilteredGraph & file_command_graph, const Context & context) const override;

private:
    ingredient::File construct_object_file(const ingredient::File & source, model::Recipe &recipe, const Context &context) const;
    Result compile_command_(command::Ptr &, model::Recipe & recipe, const Context & context) const;

    Language language_;
};

} } }



#endif
