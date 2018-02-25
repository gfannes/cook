#ifndef HEADER_cook_ingredient_KeyValue_hpp_ALREADY_INCLUDED
#define HEADER_cook_ingredient_KeyValue_hpp_ALREADY_INCLUDED

#include "cook/ingredient/Base.hpp"
#include <string>
#include <optional>

namespace cook { namespace ingredient {

class KeyValue : public Base<std::string>
{
public:
    explicit KeyValue(const std::string & key)
        : Base<std::string>(key)
    {
    }

    explicit KeyValue(const std::string & key, const std::string & value)
        : Base<std::string>(key),
          value_(value)
    {
    }

    bool operator==(const KeyValue & rhs) const
    {
        return equal_(rhs) && value_ == rhs.value_;
    }

    Result merge(const KeyValue & rhs)
    {
        MSS_BEGIN(Result);

        MSS(merge_(*this, rhs));

        value_ = rhs.value_;

        MSS_END();
    }

    bool has_value() const                      { return value_.has_value(); }
    const std::string & value() const           { return *value_; }
    bool set_value(const std::string & value)   { value_ = value; }
    bool clear_value()                          { value_.reset(); }

private:
    std::optional<std::string> value_;
};

inline std::ostream &operator<<(std::ostream &os, const KeyValue & key_value)
{
    os << "Key" <<  key_value.key() << " |";

    if (key_value.has_value())
        os << " " << key_value.value();

    return os;
}

} }

#endif
