#ifndef LYNX_UTILS_PATH_H
#define LYNX_UTILS_PATH_H

#include <string>

namespace Lynx::Utils {

    inline std::string GetFileName(const std::string path) {
        size_t fname_index = path.find_last_of("/")+1;
        return path.substr(fname_index);
    }

    inline std::string GetFileExtension(const std::string path) {
        size_t fname_index = path.find_last_of(".")+1;
        return path.substr(fname_index);
    }

    inline std::string GetFilePath(const std::string path) {
        return path.substr(0, path.find_last_of("/"));
    }

    inline std::string GetPosixLibraryPath(const std::string library) {
        std::string lname = GetFileName(library);
        std::string lpath = GetFilePath(library);

        if(lpath == library) {
            lpath = "";
        }

        return lpath + "lib" + lname + ".so";
    }

    inline std::string GetWindowsLibraryPath(const std::string library) {
        std::string lname = GetFileName(library);
        std::string lpath = GetFilePath(library);

        return lpath + lname + ".dll";
    }

    inline std::string GetAbsolutePath(const std::string path, const std::string input) {
        if(input.rfind(".", 0) == 0)
            return path + input.substr(1);
        
        return input;
    }

}

#endif // LYNX_UTILS_PATH_H