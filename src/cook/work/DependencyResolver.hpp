#ifndef HEADER_cook_work_DependencyResolver_hpp_ALREADY_INCLUDED
#define HEADER_cook_work_DependencyResolver_hpp_ALREADY_INCLUDED

#include "cook/structure/Book.hpp"
#include "cook/structure/Recipe.hpp"
#include <vector>

namespace cook { namespace work {
    
    struct DependencyResolver 
    {
        using Book      = structure::Book;
        using Recipe    = structure::Recipe;
        using Tag       = structure::Tag;
        using Uri       = structure::Uri;
        
        using recipe_iterator = std::vector<Recipe *>::const_reverse_iterator;
        
        using unresolved_dependency = std::pair<Recipe *, Uri>;
        
        bool resolve(Book & root);
        bool resolve(Book & root, std::list<unresolved_dependency> & lst );
        
        
        
        const std::vector<Recipe*> & order() const { return order_; }
        
        
        
    private:
        using RecipeMap = std::unordered_map<Uri, Recipe *>;
        
        Recipe * find_dependency_(const Recipe & recipe, const Uri & dependency, const RecipeMap & map) const;
        
        bool construct_topological_order_(Book & root, const RecipeMap & map);
        void propagate_merge_();
        std::vector<Recipe *> order_;
    };

} }

#endif
