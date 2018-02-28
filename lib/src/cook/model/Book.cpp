#include "cook/model/Book.hpp"
#include "cook/model/Recipe.hpp"
#include "gubg/mss.hpp"

namespace cook { namespace model {

Book::Book()
    : Element(make_root_uri())
{
}

Book::Book(const Uri & uri)
    : Element(uri)
{
}

bool Book::is_root() const
{
    return uri().path().empty();
}

gubg::Range<Book::BookIterator> Book::books() const
{
    return gubg::make_range(
                boost::make_transform_iterator<ExtractPointer>(subbooks_.begin()),
                boost::make_transform_iterator<ExtractPointer>(subbooks_.end())
                );
}
gubg::Range<Book::RecipeIterator> Book::recipes() const
{
    return gubg::make_range(
                boost::make_transform_iterator<ExtractPointer>(recipes_.begin()),
                boost::make_transform_iterator<ExtractPointer>(recipes_.end())
                );
}

Book * Book::find_book(const Part & part) const
{
    auto it = subbooks_.find(part);
    return (it == subbooks_.end() ? nullptr : it->second.get());
}

Recipe * Book::find_recipe(const Part & part) const
{
    auto it = recipes_.find(part);
    return (it == recipes_.end() ? nullptr : it->second.get());
}

Book & Book::goc_book(const Part & part)
{
    auto it = subbooks_.find(part);
    if (it == subbooks_.end())
    {
        Uri child_uri = uri();
        child_uri.add_path_part(part);
        auto ptr = std::make_shared<Book>(child_uri);

        ptr->set_parent(this);

        it = subbooks_.insert( std::make_pair(part, ptr) ).first;
    }

    return *(it->second);
}

Recipe & Book::goc_recipe(const Part & part)
{
    auto it = recipes_.find(part);
    if (it == recipes_.end())
    {
        auto ptr = std::make_shared<Recipe>(this, part);
        it = recipes_.insert( (std::make_pair(part, ptr)) ).first;
    }

    return *(it->second);
}

Result Book::find_relative(Recipe *& result, const Uri & uri, Book * relative)
{
    MSS_BEGIN(Result);

    result = nullptr;

    MSS(!!relative);
    MSG_MSS(uri.has_name(), Error, "The uri " << uri << " is not a valid recipe uri");
    MSG_MSS(!uri.absolute(), Error, "The uri '" << uri << " is not relative");

    Book * parent = nullptr;
    MSS(find_relative(parent, uri.parent(), relative));

    if (parent)
        result = parent->find_recipe(*uri.name());

    MSS_END();
}

Result Book::find_relative(Book *& result, const Uri & uri, Book * relative)
{
    MSS_BEGIN(Result);

    result = nullptr;

    MSS(!!relative);
    MSG_MSS(!uri.has_name(), Error, "The uri " << uri << " is not a valid book uri");
    MSG_MSS(!uri.absolute(), Error, "The uri '" << uri << " is not relative");

    Book * current = relative;
    for(const auto & p : uri.path())
    {
        Book * child = current->find_book(p);
        if (!child)
            MSS_RETURN_OK();

        current = child;
    }

    result = current;

    MSS_END();
}

Result Book::goc_relative(Recipe *& result, const Uri & uri, Book * relative)
{
    MSS_BEGIN(Result);

    result = nullptr;

    MSS(!!relative);
    MSG_MSS(uri.has_name(), Error, "The uri " << uri << " is not a valid recipe uri");
    MSG_MSS(!uri.absolute(), Error, "The uri '" << uri << " is not relative");

    Book * parent = nullptr;
    MSS(goc_relative(parent, uri.parent(), relative));
    MSS(!!parent);

    result = &parent->goc_recipe(*uri.name());
    MSS(!!result);

    MSS_END();
}

Result Book::goc_relative(Book *& result, const Uri & uri, Book * relative)
{
    MSS_BEGIN(Result);

    result = nullptr;

    MSS(!!relative);
    MSG_MSS(!uri.has_name(), Error, "The uri " << uri << " is not a valid book uri");
    MSG_MSS(!uri.absolute(), Error, "The uri '" << uri << " is not relative");

    result = relative;
    for(const auto & p : uri.path())
        result = &result->goc_book(p);

    MSS_END();
}

} }

