#ifndef LYNX_UTILS_PATH_H
#define LYNX_UTILS_PATH_H

#include <string>

namespace Lynx::Utils {

    std::string GetFileName(std::string path) {
        size_t fname_index = path.find_last_of("/")+1;
        return path.substr(fname_index);
    }

    std::string GetFilePath(std::string path) {
        return path.substr(0, path.find_last_of("/"));
    }

    std::string GetPosixLibraryPath(std::string library) {
        std::string lname = GetFileName(library);
        std::string lpath = GetFilePath(library);

        if(lpath == library) {
            lpath = "";
        }

        return lpath + "lib" + lname + ".so";
    }

    std::string GetWindowsLibraryPath(std::string library) {
        std::string lname = GetFileName(library);
        std::string lpath = GetFilePath(library);

        return lpath + lname + ".dll";
    }

}

#endif // LYNX_UTILS_PATH_H