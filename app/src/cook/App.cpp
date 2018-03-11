#include "cook/App.hpp"
#include "cook/process/chef/CompileLinkArchive.hpp"
#include "cook/algo/Book.hpp"
#include "cook/util/File.hpp"
#include "gubg/mss.hpp"
#include <unordered_set>

namespace  { 
const char *logns = nullptr;
} 

namespace cook {

bool App::initialize(const app::Options & options)
{
    MSS_BEGIN(bool);
    MSS(options.valid());

    options_ = options;
    MSS_END();
}

Result App::process()
{
    MSS_BEGIN(Result, logns);

    log_.set_level(options_.verbosity);

    auto scope = log_.scope("process", 3);

    options_.stream(scope, 2);

    // initialize the kitchen
    MSS(kitchen_.initialize());

    // set all the variables
    for(const auto & v : options_.variables)
        MSS(kitchen_.set_variable(v.first, v.second));

    {
        auto scop = scope.scope("Loading recipes", 3);
        // process all files
        MSS(load_recipes_());
    }

    std::list<model::Recipe*> root_recipes;
    {
        auto scop = scope.scope("root_recipes", 3);
        MSS(extract_root_recipes_(root_recipes));
        for (auto rr: root_recipes)
            rr->stream(scop);
    }

    {
        auto scop = scope.scope("menu", 3);
        Result rc = kitchen_.initialize_menu(root_recipes);

        if (!rc)
            MSS(process_generators_());

        kitchen_.menu().stream(scop, 3);

        MSS(rc);
    }

    {
        auto scop = scope.scope("Process with chef", 3);
        // process the menu with the chef
        {
            process::chef::LinkArchiveChef lac("default");
            MSS(lac.initialize());

            MSS(lac.mis_en_place(kitchen_));
        }
    }

    {
        auto scop = scope.scope("Process with generators", 3);
        // and now process all the requested visualizations
        MSS(process_generators_());
    }

    MSS_END();
}

Result App::process_generator_(const std::string & name, const std::string & value) const
{
    MSS_BEGIN(Result);

    auto scope = log::Scope::top->scope("process_generator", 3);
    scope.attr("name", name).attr("value", value);

    Context::GeneratorPtr ptr = kitchen_.get_generator(name);
    MSG_MSS(!!ptr, Error, "unknown visualizer '" << name << "'");

    MSS(ptr->set_option(value));

    if (ptr->can_process(kitchen_))
    {
        std::ofstream ofs;
        std::filesystem::path path = ptr->output_filename(kitchen_.dirs());
        MSS(util::open_file(path, ofs));
        MSS(ptr->process(ofs, kitchen_));
    }

    MSS_END();
}


Result App::process_generators_() const
{
    MSS_BEGIN(Result);

    Result rc;
    for(const auto & p: options_.generators)
        rc.merge(process_generator_(p.first, p.second));

    MSS(rc);

    MSS_END();
}

Result App::load_recipes_()
{
    MSS_BEGIN(Result);

    if (options_.recipe_files.empty())
        options_.recipe_files.push_back("./");

    for(const auto & fn : options_.recipe_files)
        MSS(kitchen_.load(fn));

    MSS_END();
}

Result App::extract_root_recipes_(std::list<model::Recipe*> & result) const
{
    MSS_BEGIN(Result);

    if (options_.recipes.empty())
    {
        result = kitchen_.lib().list_all_recipes();
        MSS_RETURN_OK();
    }
    else
    {
        // try to find all recipes
        Result rc;
        for(const auto & v : options_.recipes)
        {
            model::Recipe * recipe = nullptr;
            rc.merge(kitchen_.find_recipe(recipe, v));

            if (recipe)
                result.push_back(recipe);
        }
        MSS(rc);
    }

    MSS_END();
}

}

