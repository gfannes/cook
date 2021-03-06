#include "cook/App.hpp"
#include "cook/process/chef/CompileArchiveLink.hpp"
#include "cook/algo/Book.hpp"
#include "cook/util/File.hpp"
#include "cook/log/Scope.hpp"
#include "cook/generator/Interface.hpp"
#include "gubg/mss.hpp"
#include <unordered_set>

namespace  { 
/* const char *logns = "App"; */
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

bool App::process()
{
    MSS_BEGIN(bool);

    Result rc = process_();
    write_(rc);

    MSS(rc);

    MSS_END();
}

Result App::process_()
{
    MSS_BEGIN(Result, logns);

    log::set_level(options_.verbosity);

    auto ss = log::scope("App::process", -2, [&](auto &n){n.attr("verbosity", options_.verbosity);});

    options_.stream();

    // set the directories
    kitchen_.dirs().set_output(options_.output_path);
    kitchen_.dirs().set_temporary(options_.temp_path);
    for(const auto & d : options_.include_dirs)
        kitchen_.dirs().add_include_dir(d);
    

    // load the toolchains from file
    MSS(load_toolchains_());

    // Set the toolchain options
    for(const auto & p : options_.toolchain_options)
    {
        if (!p.second)
            kitchen_.add_toolchain_config(p.first);
        else
            kitchen_.add_toolchain_config(p.first, *p.second);
    }

    // initialize the kitchen
    MSS(kitchen_.initialize());

    // set all the variables
    for(const auto & v : options_.variables)
        MSS(kitchen_.set_variable(v.first, (!!v.second ? *v.second : "")));

    {
        auto ss = log::scope("Loading recipes", -2);
        // process all files
        MSS(load_recipes_());
    }

    std::list<model::Recipe*> root_recipes;
    {
        auto ss = log::scope("Extracting root recipes", -2);
        MSS(extract_root_recipes_(root_recipes));
        for (auto rr: root_recipes)
            rr->stream();
    }


    // set the schef
    std::shared_ptr<process::chef::Interface> chef;
    {
        auto ss = log::scope("Process with chef", -2);
        //Process the menu with the chef
        {
            auto chef_name = options_.chef;
            if (chef_name.empty())
                //CompileArchiveLink chef is default, with script execution
                chef_name = "scal";

            if (false) {}
            else if (chef_name == "void")
            {
                //No chef. This is useful when only the recipe names are needed, without the timeconsuming file resolving etc.
            }
            else if (chef_name == "scal")
            {
                chef = std::make_shared<process::chef::CompileArchiveLink>(true);
            }
            else if (chef_name == "cal")
            {
                chef = std::make_shared<process::chef::CompileArchiveLink>(false);
            }
        }
    }

    // try resolving
    Result resolve_result;
    {
        auto ss = log::scope("Preparing menu", -2);
        resolve_result = kitchen_.initialize_menu(root_recipes);
    }
    
    // dependening the chef, we will respond differently
    if (chef)
    {
        if (resolve_result)
        {
            auto ss = log::scope("Processing the chef", -2);
            MSS(chef->initialize());
            MSS(chef->mis_en_place(kitchen_));
        }
    
        // process the generators
        MSS(process_generators_());
        kitchen_.menu().stream();

        // ouput if there were unresolved dependencies
        MSS(resolve_result);
    }
    else
    {
        // process the generators
        MSS(process_generators_());
        kitchen_.menu().stream();
    }


    MSS_END();
}

Result App::process_generator_(const std::string & name, const std::optional<std::string> & value) const
{
    MSS_BEGIN(Result);

    auto ss = log::scope("process_generator", -2, [&](auto &n){n.attr("name", name).attr("value", (!!value ? *value : ""));});

    Context::GeneratorPtr ptr = kitchen_.get_generator(name);
    MSG_MSS(!!ptr, Error, "unknown generator '" << name << "'");

    if (value)
        MSS(ptr->set_option(*value));

    if (ptr->can_process(kitchen_))
    {
        MSS(ptr->process(kitchen_));
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

Result App::load_toolchains_()
{
    MSS_BEGIN(Result);

    if (options_.toolchains.empty())
        options_.toolchains.push_back("default");

    for(const auto & fn : options_.toolchains)
        MSS(kitchen_.load_toolchain(fn));

    MSS_END();
}

Result App::load_recipes_()
{
    MSS_BEGIN(Result);

    if (options_.recipe_files.empty())
        options_.recipe_files.push_back("./");

    for(const auto & fn : options_.recipe_files)
        MSS(kitchen_.load_recipe(fn));

    MSS_END();
}

Result App::extract_root_recipes_(std::list<model::Recipe*> & result) const
{
    MSS_BEGIN(Result);

    if (options_.recipes.empty())
    {
        //No recipes are specified by the user, we return all of them
        result = kitchen_.lib().list_all_recipes();
        MSS_RETURN_OK();
    }
    else
    {
        //Try to find all recipes specified by the user
        Result rc;
        for(const auto & rcp : options_.recipes)
        {
            model::Recipe * recipe = nullptr;
            rc.merge(kitchen_.find_recipe(recipe, rcp));

            if (recipe)
                result.push_back(recipe);
        }
        MSS(rc);
    }

    MSS_END();
}

void App::write_(const Result & result)
{
    result.each_message([&](const Message & msg) {

        bool should_print = false;

        switch(msg.type_)
        {
        case Message::Info:
            should_print = options_.verbosity > 2;
            break;
        case Message::Warning:
            should_print = options_.verbosity > 1;
            break;
        case Message::Error:
        case Message::InternalError:
            should_print = true;
            break;

        default:
            should_print = false;
        }

        if (should_print)
        {
            std::cout << msg.type_ << ": " << msg.msg_ << std::endl;
            for(log::Ptr p = msg.node_; p; p = p->parent())
                std::cout << "  * " << p->header() << std::endl;
        }
    });
}

}

