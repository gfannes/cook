#ifndef HEADER_cook_process_Menu_hpp_ALREADY_INCLUDED
#define HEADER_cook_process_Menu_hpp_ALREADY_INCLUDED

#include "cook/process/build/Graph.hpp"
#include "cook/process/RecipeFilteredGraph.hpp"
#include "cook/model/Recipe.hpp"
#include "cook/model/Book.hpp"
#include <unordered_map>

namespace cook { namespace process {

class Menu
{
public:
    Menu();

    template <typename It>
    Result construct(const gubg::Range<It> & range)
    {
        root_recipes_.assign(RANGE(range));
        return construct_();
    }


    bool is_valid() const { return valid_; }

    const std::list<model::Recipe*> & topological_order() const;
    const std::list<model::Recipe*> & root_recipes() const;

private:
    Menu(const Menu &) = delete;
    Menu(Menu &&) = delete;
    Menu & operator=(const Menu &) = delete;
    Menu & operator=(Menu &&) = delete;

    using CountMap = std::unordered_map<model::Recipe *, unsigned int>;
    Result construct_();
    bool initialize_process_info_();
    bool grow_(model::Recipe * seed, build::GraphPtr graph_ptr, const std::unordered_multimap<model::Recipe *, model::Recipe *> & in_edge_map);

    std::list<model::Recipe *> topological_order_;
    std::list<model::Recipe *> root_recipes_;

    struct ProcessInfo
    {
        ProcessInfo(build::GraphPtr ptr): graph(ptr) {}

        RecipeFilteredGraph graph;
    };

    std::map<model::Recipe *, ProcessInfo> process_info_map_;
    bool valid_;
};

} }

#endif
