#ifndef HEADER_cook_chai_Flags_hpp_ALREADY_INCLUDED
#define HEADER_cook_chai_Flags_hpp_ALREADY_INCLUDED

#include "cook/Language.hpp"
#include "cook/Type.hpp"
#include "cook/TargetType.hpp"
#include "cook/Overwrite.hpp"
#include "cook/Propagation.hpp"
#include "cook/chai/mss.hpp"

namespace cook { namespace chai {

enum class Flag
{
    Language,
    Type,
    TargetType,
    Overwrite,
    Propagation,
    _End
};

std::ostream & operator<<(std::ostream & str, Flag flag);

class Flags
{
public:
    Flags();

    template <typename T> Flags(T val) { set(val); }

    Flags & operator|(const Flags & rhs);
    bool is_set(Flag flag) const;

#define SET(TYPE) void set(TYPE value) { flags_ |= (1u << static_cast<int>(Flag::TYPE)); t##TYPE##_ = value; }
    SET(Language)
    SET(Type)
    SET(TargetType)
    SET(Overwrite)
    SET(Propagation)
#undef SET

#define GET_OR(TYPE) TYPE get_or(TYPE fallback) const { return is_set(Flag::TYPE) ? t##TYPE##_ : fallback; }
    GET_OR(Language)
    GET_OR(Type)
    GET_OR(TargetType)
    GET_OR(Overwrite)
    GET_OR(Propagation)
#undef GET_OR

#define GET(TYPE, NAME) std::pair<TYPE, bool> NAME() const{ return std::make_pair(t##TYPE##_, is_set(Flag::TYPE)); }
    GET(Language, language)
    GET(Type, type)
    GET(TargetType, target_type)
    GET(Overwrite, overwrite)
    GET(Propagation, propagation)
#undef GET

    Result only(const std::initializer_list<Flag> & lst) const;

private:
    unsigned int flags_;
    Language tLanguage_;
    Type tType_;
    TargetType tTargetType_;
    Overwrite tOverwrite_;
    Propagation tPropagation_;
};


} }

#endif

