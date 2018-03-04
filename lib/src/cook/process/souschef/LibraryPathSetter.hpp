#ifndef HEADER_cook_process_souschef_LibraryPathSetter_hpp_ALREADY_INCLUDED
#define HEADER_cook_process_souschef_LibraryPathSetter_hpp_ALREADY_INCLUDED

#include "cook/process/souschef/Interface.hpp"

namespace cook { namespace process { namespace souschef {

class LibraryPathSetter : public Interface
{
public:
    std::string description() const override { return "Library path setter"; }
    Result process(model::Recipe & recipe, RecipeFilteredGraph & file_command_graph, const Context & context) const override;
};

} } }

#endif