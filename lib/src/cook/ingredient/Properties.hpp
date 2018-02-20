#ifndef HEADER_cook_ingredient_Properties_hpp_ALREADY_INCLUDED
#define HEADER_cook_ingredient_Properties_hpp_ALREADY_INCLUDED

#include "cook/ingredient/Collection.hpp"
#include "cook/LanguageTypePair.hpp"
#include "gubg/mss.hpp"
#include <map>

namespace cook { namespace ingredient {

template <typename Ingredient> class Ingredients
{
    using IngredientMap = std::map<LanguageTypePair, Collection<Ingredient>>;

public:
    using iterator = typename IngredientMap::iterator;
    using const_iterator = typename IngredientMap::const_iterator;

    bool insert_or_merge(const LanguageTypePair & key, const Ingredient & ingredient)
    {
        MSS_BEGIN(bool);

        auto p = insert(key, ingredient);
        if (!p.second)
            MSS(p.first->merge(ingredient));

        MSS_END();
    }

    std::pair<typename Collection<Ingredient>::iterator, bool> insert(const LanguageTypePair & collection_key, const Ingredient & ingredient)
    {
        // add if necessary
        Collection<Ingredient> & collection = ingredients_[collection_key];
        return collection.insert(ingredient);
    }

    Collection<Ingredient> & operator[](const LanguageTypePair & key)
    {
        return ingredients_[key];
    }

    iterator begin()                { return ingredients_.begin(); }
    iterator end()                  { return ingredients_.end(); }
    const_iterator begin() const    { return ingredients_.begin(); }
    const_iterator end() const      { return ingredients_.end(); }

    iterator find(const LanguageTypePair & collection_key)              { return ingredients_.find(collection_key); }
    const_iterator find(const LanguageTypePair & collection_key) const  { return ingredients_.find(collection_key); }

    Ingredient * find(const LanguageTypePair & collection_key, const typename Ingredient::key_type & ingredient_key)
    {
        return find_<Ingredient>(collection_key, ingredient_key, ingredients_);
    }

    const Ingredient * find(const LanguageTypePair & collection_key, const typename Ingredient::key_type & ingredient_key) const
    {
        return find_<const Ingredient>(collection_key, ingredient_key, ingredients_);
    }

    template <typename Functor>
    bool each(Functor && functor) const
    {
        for(const auto & p : ingredients_)
            for(const auto & file : p.second)
                if (!functor(p.first, file))
                    return false;

        return true;
    }


private:
    template <typename Ingredient_, typename Ingredients_>
    Ingredient_ * find_(const LanguageTypePair & collection_key, const typename Ingredient::key_type & ingredient_key, Ingredients_ & ingredients) const
    {
        auto pit = ingredients.find(collection_key);
        if (pit == ingredients.end())
            return nullptr;


        auto & collection = pit->second;
        auto cit = collection.find(ingredient_key);

        if (cit == collection.end())
            return nullptr;

        return &*cit;
    }

    IngredientMap ingredients_;
};

} }

#endif