#include "cook/view/chai/File.hpp"
#include "cook/File.hpp"
#include "chaiscript/chaiscript.hpp"


namespace cook { namespace view { namespace chai {

ModulePtr file_module()
{
    ModulePtr m(new chaiscript::Module());

    chaiscript::utility::add_class<FileType>(*m,
          "FileType",
          { { Unknown, "unknown" }, { Force_Include, "force_include" }, { Source, "source" }, { Header, "header" } }
          );

    m->add(chaiscript::user_type<File>(), "File");
    m->add(chaiscript::fun(&File::dir_part), "dir");
    m->add(chaiscript::fun(&File::rel_part), "relative");
    m->add(chaiscript::fun(&File::extension), "extension");
    m->add(chaiscript::fun(&File::filename), "filename");
    m->add(chaiscript::fun(&File::type), "type");

    return m;
}

} } }