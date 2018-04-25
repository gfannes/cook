#ifndef HEADER_cook_chai_Recipe_hpp_ALREADY_INCLUDED
#define HEADER_cook_chai_Recipe_hpp_ALREADY_INCLUDED

#include "cook/chai/UserData.hpp"
#include "cook/chai/UserData.hpp"
#include "cook/chai/Flags.hpp"
#include "cook/model/Recipe.hpp"
#include "cook/Logger.hpp"
#include <functional>

namespace cook { namespace chai {

class Context;
class File;

class Recipe
{
public:
    using GlobFunctor = std::function<bool (File &)>;
    using DepFileFilter = model::Recipe::DependencyFileFilter;
    using DepKeyValueFilter = model::Recipe::DependencyKeyValueFilter;


    Recipe(model::Recipe * recipe);

    void add(const std::string & dir, const std::string & pattern, const Flags & flags = Flags(), GlobFunctor functor = GlobFunctor());
    void remove(const std::string & dir, const std::string & pattern, const Flags & flags = Flags(), GlobFunctor functor = GlobFunctor());
    void depends_on(const std::string & dependency, const DepFileFilter & file_filter = DepFileFilter(), const DepKeyValueFilter & key_value_filter = DepKeyValueFilter());
    void set_type(TargetType type);
    void set_working_directory(const std::string & dir);

    void library(const std::string & library, const Flags & flags = Flags());
    void library_path(const std::string & path, const Flags & flags= Flags());
    void include_path(const std::string & path, const Flags & flags= Flags());
    void define(const std::string & name, const Flags & flags= Flags());
    void define(const std::string & name, const std::string & value, const Flags & flags = Flags());
    void add_script(const std::string & script_fn, const std::list<std::string> & args);
    const model::Uri & uri() const { return recipe_->uri(); }

    UserData data() const { return data_; }

private:
    model::Recipe * recipe_;
    UserData data_;
};

} }

#endif
