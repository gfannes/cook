#include "cook/view/chai/Recipe.hpp"
#include "cook/view/Recipe.hpp"
#include "chaiscript/chaiscript.hpp"

namespace cook { namespace view { namespace chai {

ModulePtr recipe_module()
{
    ModulePtr m(new chaiscript::Module());

    m->add_global_const(chaiscript::const_var(Overwrite::Always), "overwrite_always");
    m->add_global_const(chaiscript::const_var(Overwrite::Never), "overwrite_never");
    m->add_global_const(chaiscript::const_var(Overwrite::IfSame), "overwrite_if_same");

    m->add(chaiscript::user_type<Recipe>(), "Recipe");
    m->add(chaiscript::constructor<Recipe(const Recipe &)>(), "Recipe");
    m->add(chaiscript::fun(&Recipe::print), "print");
    m->add(chaiscript::fun(&Recipe::add_select_3), "add");
    m->add(chaiscript::fun(&Recipe::add_select_2), "add");
    m->add(chaiscript::fun(&Recipe::add_select_1), "add");
    m->add(chaiscript::fun(&Recipe::add_3), "add");
    m->add(chaiscript::fun(&Recipe::add_2), "add");
    m->add(chaiscript::fun(&Recipe::add_1), "add");
    m->add(chaiscript::fun(&Recipe::depends_on), "depends_on");
    m->add(chaiscript::fun(&Recipe::display_name), "display_name");
    m->add(chaiscript::fun(&Recipe::library), "library");
    m->add(chaiscript::fun(&Recipe::library_path), "library_path");
    m->add(chaiscript::fun(&Recipe::define_K_1), "define");
    m->add(chaiscript::fun(&Recipe::define_K_2), "define");
    m->add(chaiscript::fun(&Recipe::define_KV_2), "define");
    m->add(chaiscript::fun(&Recipe::define_KV_3), "define");


    return m;
}

} } }