#ifndef HEADER_cook_model_Uri_hpp_ALREADY_INCLUDED
#define HEADER_cook_model_Uri_hpp_ALREADY_INCLUDED

#include "gubg/Range.hpp"
#include <string>
#include <list>
#include <utility>
#include <optional>

namespace cook { namespace model {

class Uri;

class Part
{
public:
    Part(const Part &) = default;
    Part(Part &&) = default;
    Part & operator=(const Part &) = default;
    Part & operator=(Part &&) = default;

    static std::optional<Part> make_part(const std::string & part);

    const std::string & string() const { return value_; }
    bool empty() const { return value_.empty(); }

    bool operator==(const Part & rhs) const { return value_ == rhs.value_; }
    bool operator<(const Part & rhs) const { return value_ < rhs.value_; }

private:
    Part(const std::string & value);

    std::string value_;
};

class Uri
{
    using PathContainer = std::list<Part>;

public:
    using iterator = PathContainer::const_iterator;

    static const char separator;

    Uri() : absolute_(false) {}

    static std::pair<Uri, bool> recipe_uri(const std::string & uri_str);
    static std::pair<Uri, bool> book_uri(const std::string & uri_str);

    void clear();

    void add_path_part(const Part & part);
    gubg::Range<iterator> path() const;
    bool pop_back(Part & part);
    bool pop_back();

    bool set_name(const std::string & name);
    bool has_name() const;
    void clear_name();

    bool absolute() const;
    void set_absolute(bool is_absolute);

    std::string string(const char sep = separator) const;


private:
    bool add_path_part_(const std::string &part);

    PathContainer path_;
    std::string name_;
    bool absolute_;
};

Uri make_root_uri();

} }

#endif
