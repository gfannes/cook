#include "cook/chai/UserData.hpp"
#include "gubg/chai/Module.hpp"

namespace cook { namespace chai {

UserData & from_any(std::any & any)
{
    if (!any.has_value())
        any.emplace<UserData>();

    return std::any_cast<UserData&>(any);
}

struct UserData::D : public chaiscript::dispatch::Dynamic_Object
{
    D()
        : chaiscript::dispatch::Dynamic_Object("cook_chai_UserData")
    {
    }

    D(const D & rhs)
        : chaiscript::dispatch::Dynamic_Object("cook_chai_UserData")
    {
        set_explicit(rhs.is_explicit());

        for(const auto & v : rhs.get_attrs())
            this->get_attr(v.first) = v.second;
    }
};

UserData::UserData()
    : d_(std::make_shared<D>())
{

}
UserData::~UserData() = default;


UserData UserData::clone() const
{
    UserData d;
    d.d_ = std::make_shared<D>(*d_);

    return d;
}

bool UserData::set_variable(const std::string & name, const std::string & value)
{
    if (d_->has_attr(name))
        return false;

    (*d_)[name] = chaiscript::var(value);
    return true;
}


bool UserData::operator==(const UserData & rhs) const
{
    return d_ == rhs.d_;
}


gubg::chai::ModulePtr user_data_module()
{
    gubg::chai::ModulePtr ptr = std::make_unique<chaiscript::Module>();

    ptr->add(chaiscript::user_type<UserData>(), "UserData");

    ptr->add(chaiscript::fun([](const UserData & data){ return data.d_->get_attrs(); }),                                    "get_attrs");
    ptr->add(chaiscript::fun([](UserData & data, bool v) { data.d_->set_explicit(v); }),                                    "set_explicit");
    ptr->add(chaiscript::fun([](const UserData & data) { return data.d_->is_explicit(); }),                                 "is_explicit");
    ptr->add(chaiscript::fun([](const UserData & data, const std::string & n) { return data.d_->has_attr(n); }),            "has_attr");
    ptr->add(chaiscript::fun([](const UserData & data, const std::string & n) { return data.d_->get_attr(n); }),            "get_attr");
    ptr->add(chaiscript::fun([](UserData & data, const std::string & n) { return data.d_->get_attr(n); }),                  "get_attr");
    ptr->add(chaiscript::fun([](const UserData & data, const std::string & n) { return data.d_->method_missing(n); }),      "method_missing");
    ptr->add(chaiscript::fun([](UserData & data, const std::string & n) { return data.d_->method_missing(n); }),            "method_missing");
    ptr->add(chaiscript::fun([](UserData & data, const std::string & n) { return data.d_->get_attr(n); }),                  "[]");
    ptr->add(chaiscript::fun([](const UserData & data, const std::string & n) { return data.d_->get_attr(n); }),            "[]");

    return ptr;
}


} }
