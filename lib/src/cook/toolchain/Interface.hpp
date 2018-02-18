#ifndef HEADER_cook_toolchain_Interface_hpp_ALREADY_INCLUDED
#define HEADER_cook_toolchain_Interface_hpp_ALREADY_INCLUDED

#include "cook/agents/Interface.hpp"
#include <memory>

namespace cook { namespace toolchain {


using AgentPtr = std::shared_ptr<agents::Interface>;

struct Step
{
    std::string name;
    std::string description;
    std::vector<AgentPtr> agents;
};

class Interface
{
public:
    virtual bool can_process(model::Recipe * recipe) const = 0;
    virtual std::list<Step> generate_processors(model::Recipe * recipe) = 0;
};

} }

#endif