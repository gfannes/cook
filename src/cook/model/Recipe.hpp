#ifndef HEADER_cook_model_Recipe_hpp_ALREADY_INCLUDED
#define HEADER_cook_model_Recipe_hpp_ALREADY_INCLUDED

#include "gubg/file/System.hpp"
#include "gubg/macro/capture.hpp"
#include <set>
#include <ostream>

namespace cook { namespace model { 

    class Book;

    enum class FileType {Unknown, Source, Header};
    std::ostream &operator<<(std::ostream &, FileType);

    struct File
    {
        std::filesystem::path dir;
        std::filesystem::path path;
        FileType type = FileType::Unknown;
        std::string language;

        void stream(std::ostream &os) const
        {
            os << C(path)C(dir)C(type)C(language);
        }
    };
    using FilePerPath = std::map<std::filesystem::path, File>;

    class Recipe
    {
    public:
        const std::string &name() const {return name_;}
        const std::string &type() const {return type_;}

        bool set(const std::string &key, const std::string &value)
        {
            MSS_BEGIN(bool);
            if (false) { }
            else if (key == "type") { MSS(value.empty() || value == "executable"); type_ = value; }
            else if (key == "working_directory") { wd_ = value; }
            else if (key == "depends_on") { deps_.insert(value); }
            MSS_END();
        }

        void add(std::string p_dir, const std::string &pattern)
        {
            if (p_dir.empty())
                p_dir = ".";

            std::filesystem::path dir = p_dir;
            if (dir.is_relative())
                dir = wd_ / dir;

            auto add_file = [&](const std::filesystem::path &fp)
            {
                auto &file = file_per_path_[fp];
                file.dir = dir;
                file.path = fp;
                const auto ext = fp.extension();
                if (false) {}
                else if (ext == ".c")   {file.type = FileType::Source; file.language = "c";}
                else if (ext == ".h")   {file.type = FileType::Header; file.language = "c";}
                else if (ext == ".cpp") {file.type = FileType::Source; file.language = "c++";}
                else if (ext == ".hpp") {file.type = FileType::Header; file.language = "c++";}
                else if (ext == ".asm") {file.type = FileType::Source; file.language = "asm";}
            };
            gubg::file::each_glob(pattern, add_file, dir);
        }

        void stream(std::ostream &os) const
        {
            os << name_ << ", type: " << type_ << ", working directory: " << wd_ << ", nr files: " << file_per_path_.size() << ", nr deps: " << deps_.size() << std::endl;
            for (const auto &p: file_per_path_)
            {
                os << "\tFile: "; p.second.stream(os); os << std::endl;
            }
            for (const auto &dep: deps_)
            {
                os << "\tDep: " << dep << std::endl;
            }
        }

    private:
        friend class Book;

        std::string name_;
        std::string type_;
        std::filesystem::path wd_;
        FilePerPath file_per_path_;
        std::set<std::string> deps_;
    };

} } 

#endif