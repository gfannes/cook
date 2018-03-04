#ifndef HEADER_cook_process_RecipeFilteredGraph_hpp_ALREADY_INCLUDED
#define HEADER_cook_process_RecipeFilteredGraph_hpp_ALREADY_INCLUDED

#include "cook/process/build/Graph.hpp"

namespace cook { namespace process {

class RecipeFilteredGraph : public build::config::Graph
{
    using CommandVertices = std::unordered_set<vertex_descriptor>;
    using CommandVertexIterator = CommandVertices::const_iterator;
public:
    explicit RecipeFilteredGraph(build::GraphPtr ptr);

    vertex_descriptor goc_vertex(const FileLabel & path);
    vertex_descriptor add_vertex(CommandLabel ptr);
    Result add_edge(vertex_descriptor src, vertex_descriptor tgt);

    Result topological_commands(std::vector<vertex_descriptor> & commands) const;

    const Label & operator[](vertex_descriptor vd) const;

    gubg::Range<CommandVertexIterator> command_vertices() const;

private:
    std::unordered_set<vertex_descriptor> command_vertices_;

    build::GraphPtr ptr_;
};

} }

#endif