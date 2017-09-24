#ifndef HEADER_cook_chai_Details_hpp_ALREADY_INCLUDED
#define HEADER_cook_chai_Details_hpp_ALREADY_INCLUDED

#include "cook/chai/Engine.hpp"
#include "cook/structure/Book.hpp"
#include "cook/structure/Recipe.hpp"
#include "cook/Options.hpp"
#include <algorithm>
#include <iterator>


namespace cook { namespace chai { 
    
    using namespace cook::structure;
    
    struct GlobalInfo;
    struct BookWrapper;
    struct RecipeWrapper;
    
    using ScriptStack = std::stack<std::filesystem::path>;
    using BookStack = std::stack<Book *>;
    
    
    struct GlobalInfo
    {
        GlobalInfo(const Options & opt, Book & root) 
        : options(opt)
        {
            open_books.push(&root);
        }
        
        const Options & options;
        Engine engine;
        
        BookStack open_books;
        ScriptStack open_scripts;
        
        Book & last_book()
        {
            assert(!open_books.empty());
            return *open_books.top();
        }
        
        const std::filesystem::path & last_script()
        {
            assert(!open_scripts.empty());
            return open_scripts.top();
        }
        
        // chai functionality
        bool add_dir(const std::string & dir);
        bool add_book(const std::string & ns, std::function<void (BookWrapper &)> callback);
        bool create_new_recipe(const std::string & tag, std::function<void (RecipeWrapper &)> callback);
        void set_display_name(const std::string & display_name);
        void print();
        
        unsigned int verbosity() const { return options.verbose; }
        
        std::ostream & stream() 
        { 
            std::fill_n(std::ostream_iterator<char>(std::cout), indent_, ' ');
            return std::cout;
        }
        
        void open_indent() { ++indent_; }
        void close_indent() { --indent_; }
        
        std::ostream & open_and_stream()
        {
            open_indent();
            return stream();
        }
        
        std::ostream & stream_and_close()
        {
            std::ostream & res = stream();
            close_indent();
            return res;
        }
        
    private:
        unsigned int indent_ = 0;
    };
    
    struct VerbosityWriter
    {
        VerbosityWriter(unsigned int lvl, GlobalInfo & info)
            : info_(info),
              should_write_(lvl <= info.verbosity())
        {
        }
        
        ~VerbosityWriter()
        {
            if (should_write_)
                info_.stream_and_close() << "Done" << std::endl;
        }
        
        std::ostream & str() 
        { 
            if (should_write_)
                return info_.open_and_stream();
            else
                return str_;
        }
        
    private:
        GlobalInfo & info_;
        bool should_write_;
        std::ostringstream str_;
    };
    
    
    struct RecipeWrapper
    {
        RecipeWrapper(GlobalInfo & info, Recipe & recipe) : info_(info), recipe_(recipe) {}
        
        // chai functionality
        void add_source(const std::string &dir, const std::string &pattern)     { recipe_.add_source(dir, pattern); }
        void add_header(const std::string &dir, const std::string &pattern)     { recipe_.add_header(dir, pattern); }
        void add_include_path(const std::string &dir)                           { recipe_.add_include_path(dir); }
        void add_define_1(const std::string &name)                              { recipe_.add_define(name); }
        void add_define_2(const std::string &name, const std::string &value)    { recipe_.add_define(name, value); }
        void add_library_path(const std::string &dir)                           { recipe_.add_library_path(dir); }
        void add_library(const std::string &library)                            { recipe_.add_library(library); }
        void depends_on(const std::string & uri)                                { recipe_.depends_on(Uri(uri)); }
        void print() const                                                      { recipe_.print(); }
        void set_display_name(const std::string & name)                         { recipe_.set_display_name(name); }
        void set_target_type(structure::TargetType type)                        { recipe_.set_target_type(type); }

        GlobalInfo & info_;
        Recipe & recipe_;
    };
    
    struct BookWrapper
    {
        BookWrapper(GlobalInfo & info, Book & book) : info_(info), book_(book) {}
        
        // functionality for chai
        bool create_new_recipe(const std::string & tag, std::function<void (RecipeWrapper &)> callback);
        void print() const                                                      { book_.print(); }
        void set_display_name(const std::string & name)                         { book_.set_display_name(name); }
        bool add_dir(const std::string & dir);
        bool add_book(const std::string & tag, std::function<void (BookWrapper &)> callback);
        
        
        GlobalInfo & info_;
        Book & book_;
    };

} }

#endif