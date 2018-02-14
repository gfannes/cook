#include "cook/model/Uri.hpp"
#include "gubg/Strange.hpp"
#include "gubg/mss.hpp"
#include <cassert>

namespace cook { namespace model {

const char Uri::separator = '/';

std::optional<Part> Part::make_part(const std::string & part)
{
    if (part.find(Uri::separator) != std::string::npos || part.empty())
        return {};

    return Part(part);
}

Part::Part(const std::string & value)
    : value_(value)
{
}

std::pair<Uri, bool> Uri::recipe_uri(const std::string & uri_str)
{
    std::pair<Uri, bool> res(Uri(), true);

    gubg::Strange strange(uri_str);
    res.first.absolute_ = strange.pop_if(separator);

    std::string part;
    while (strange.pop_until(part, separator))
        if (!res.first.add_path_part_(part))
            return std::make_pair(Uri(), false);

    if (!res.first.set_name(strange.str()))
        return std::make_pair(Uri(), false);

    return res;
}

std::pair<Uri, bool> Uri::book_uri(const std::string & uri_str)
{
    std::pair<Uri, bool> res(Uri(), true);

    gubg::Strange strange(uri_str);
    res.first.absolute_ = strange.pop_if(separator);

    std::string part;
    while (strange.pop_until(part, separator))
        if (!res.first.add_path_part_(part))
            return std::make_pair(Uri(), false);

    if (!strange.empty())
        if (!res.first.add_path_part_(strange.str()))
            return std::make_pair(Uri(), false);

    return res;
}

void Uri::add_path_part(const Part & part)
{
    path_.push_back(part);
}

bool Uri::add_path_part_(const std::string & part)
{
    MSS_BEGIN(bool);

    std::optional<Part> p = Part::make_part(part);
    MSS(!!p);
    path_.push_back(*p);

    MSS_END();
}

bool Uri::set_name(const std::string & name)
{
    MSS_BEGIN(bool);
    MSS(!name.empty());

    name_ = name;

    MSS_END();
}

void Uri::clear_name()
{
    name_.clear();
}

void Uri::clear()
{
    path_.clear();
    name_.clear();
    absolute_ = false;
}

bool Uri::pop_back(Part & part)
{
    MSS_BEGIN(bool);
    MSS(!path_.empty());

    part = path_.back();
    path_.pop_back();

    MSS_Q(path_.empty());
    MSS_END();
}

bool Uri::pop_back()
{
    MSS_BEGIN(bool);
    MSS(!path_.empty());

    path_.pop_back();

    MSS_Q(path_.empty());
    MSS_END();
}

gubg::Range<Uri::iterator> Uri::path() const
{
    return gubg::make_range(path_);
}

bool Uri::absolute() const
{
    return absolute_;
}

std::string Uri::string(const char sep) const
{
    std::string result;
    if (absolute_)
        result += sep;

    for(const auto & part: path_)
    {
        result += part.string();
        result += sep;
    }

    if (!name_.empty())
        result += name_;

    return result;
}

bool Uri::has_name() const
{
    return !name_.empty();
}

void Uri::set_absolute(bool is_absolute)
{
    absolute_ = is_absolute;
}

Uri make_root_uri()
{
    Uri uri;
    uri.set_absolute(true);

    return uri;
}

} }
