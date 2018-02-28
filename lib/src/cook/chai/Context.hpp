#ifndef HEADER_cook_chai_Context_hpp_ALREADY_INCLUDED
#define HEADER_cook_chai_Context_hpp_ALREADY_INCLUDED

#include "cook/chai/UserData.hpp"
#include "cook/Context.hpp"

namespace cook { namespace chai {

struct Context : public cook::Context
{
    Context();
    virtual ~Context();

    cook::Logger & logger() override;
    Result set_variable(const std::string & name, const std::string & value) override;

    bool load(const std::string &recipe);



private:
    void include_(const std::string & file);
    void reset_engine_();
    std::filesystem::path generate_file_path_(const std::string & file) const;

    struct D;
    std::unique_ptr<D> d;
    UserData data_;
};

} }

#endif

