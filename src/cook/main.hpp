#ifndef HEADER_cook_main_hpp_ALREADY_INCLUDED
#define HEADER_cook_main_hpp_ALREADY_INCLUDED

#include "cook/Codes.hpp"
#include "cook/recipe/Loader.hpp"
#include "gubg/mss.hpp"
#include <fstream>
#include <set>

namespace cook { 
    ReturnCode main(int argc, const char **argv)
    {
        MSS_BEGIN(ReturnCode);

        /* const std::string config = "debug"; */
        const std::string config = "release";

        recipe::Loader loader;
        MSS(loader.load("recipes.chai"));

        MSS(argc == 2);
        const recipe::Alias alias(argv[1]);

        const recipe::Description *ptr;
        MSS(loader.get(ptr, alias));
        const auto &description = *ptr;

        description.print();

        std::set<std::filesystem::path> include_paths;
        for (const auto &p: description.sources())
            include_paths.insert(p.second.dir);
        for (const auto &p: description.headers())
            include_paths.insert(p.second.dir);

        std::map<std::string, std::string> defines;
        if (config == "release")
            defines["NDEBUG"] = "";

        {
            std::ofstream fo("build.ninja");

            fo << "compiler = g++ -std=c++17" << std::endl;
            fo << "linker = g++ -std=c++17" << std::endl;
            fo << "delete = rm -f" << std::endl;
            fo << std::endl;

            fo << "cflags = ";
            if (config == "debug")
                fo << "-g ";
            fo << std::endl;
            fo << "defines = ";
            for (const auto &p: defines)
            {
                const auto &key = p.first;
                const auto &value = p.second;
                if (value.empty())
                    fo << "-D" << key;
                else
                    fo << "-D" << key << "=" << value;
            }
            fo << std::endl;
            fo << "include_paths =";
            for (const auto &ip: include_paths)
                fo << " -I " << ip;
            fo << std::endl;

            fo << "lflags = " << std::endl;
            fo << "libraries =";
            for (const auto &lib: description.libraries())
                fo << " -l" << lib;
            fo << std::endl;

            fo << "rule compile" << std::endl;
            fo << "  command = $compiler -c -MD -MF $out.d $cflags $defines -o $out $in $include_paths" << std::endl;
            fo << "  depfile = $out.d" << std::endl;
            fo << "rule link" << std::endl;
            fo << "  command = $linker $lflags -o $out $in $libraries" << std::endl;
            fo << std::endl;

            std::list<std::filesystem::path> objects;
            const std::filesystem::path build_dir = ".cook";
            for (const auto &p: description.sources())
            {
                const auto &source = p.first;
                auto obj = build_dir; obj /= config; obj /= source; obj += ".obj";
                objects.push_back(obj);
                fo << "build " << obj.string() << ": compile " << source.string() << std::endl;
            }

            const std::filesystem::path exe = "cook.exe";
            fo << "build " << exe.string() << ": link";
            for (const auto &obj: objects)
                fo << " $" << std::endl << "    " << obj.string();
            fo << std::endl;
            fo << std::endl;

            fo << "rule clean" << std::endl;
            fo << "  command = ninja -t clean" << std::endl;
            fo << "build clean: clean" << std::endl;

            fo << "default " << exe.string() << std::endl;
        }

        MSS_END();
    }
} 

#endif
