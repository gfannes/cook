#ifndef HEADER_cook_rules_C_family_hpp_ALREADY_INCLUDED
#define HEADER_cook_rules_C_family_hpp_ALREADY_INCLUDED

#include "cook/rules/Interface.hpp"
#include "cook/rules/Extensions.hpp"
#include "cook/LanguageTypePair.hpp"
#include "cook/ingredient/File.hpp"
#include <set>

namespace cook { namespace rules {

    class C_family : public Interface
    {
    public:
        C_family(Language lang, Extensions &extensions);

        Language language() const override;
        bool accepts_file(const LanguageTypePair & key, const ingredient::File & file) const override;
        Result resolve_file(LanguageTypePair &key, ingredient::File &file) const override;
        Result add_file(model::Recipe & recipe, const LanguageTypePair & key, const ingredient::File & file) const override;

    private:
        bool add_additional_path_(model::Recipe & recipe, const ingredient::File & file, Type type, Propagation propagation) const;

        const Language lang_;
        Extensions &extensions_;
    };

    class CXX: public C_family
    {
    public:
        CXX(): C_family(Language::CXX, extensions_){}

    private:
        static Extensions extensions_;
    };

    class Cc: public C_family
    {
    public:
        Cc(): C_family(Language::C, extensions_){}
    private:
        static Extensions extensions_;
    };
    class ObjectiveCXX: public C_family
    {
    public:
        ObjectiveCXX(): C_family(Language::ObjectiveCXX, extensions_){}

    private:
        static Extensions extensions_;
    };

    class ObjectiveCc: public C_family
    {
    public:
        ObjectiveCc(): C_family(Language::ObjectiveC, extensions_){}
    private:
        static Extensions extensions_;
    };

    class Resource: public C_family
    {
    public:
        Resource(): C_family(Language::Resource, extensions_){}
    private:
        static Extensions extensions_;
    };

} }

#endif
