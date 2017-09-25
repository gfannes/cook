#ifndef HEADER_cook_work_TreeWriter_hpp_ALREADY_INCLUDED
#define HEADER_cook_work_TreeWriter_hpp_ALREADY_INCLUDED

#include "cook/work/TopologicalOrder.hpp"
#include "cook/structure/Uri.hpp"
#include "cook/util/TreeNode.hpp"

namespace cook { namespace work {
    
    class TreeWriter
    {
    public:
        bool recipes(const Recipes &);
        bool details(const Recipes &, const structure::Uri &uri);
        
    private:
        bool write_recipe_(const structure::Recipe & recipe, util::TreeNode & node, const Recipes &, bool details);
    };
    
} }

#endif
