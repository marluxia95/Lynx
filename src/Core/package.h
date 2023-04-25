#ifndef LYNX_PACKAGE_H
#define LYNX_PACKAGE_H

#include <stdlib.h>

namespace Lynx {

    typedef struct pkg_header_s {
        char                sig[3]; // PKG
        uint8_t             version;
        uint64_t            size;
    } pkg_header_t;

    typedef struct pkg_file_s {
        char                name[512];
        uint32_t            offset;
        uint32_t            size;
        struct pkg_file_s   *next;
    } pkg_file_t;

    typedef struct pkg_dir_s {
        char                name[512];
        pkg_file_t          *files;
    } pkg_dir_t;

    bool pkg_load_file(char* filename, void** buff) {}
    bool pkg_read(char* pkgname);
    bool pkg_write(char* pkgname, char* filename, void* data, size_t size);
    void pkg_dump(char* pkgname);
}

#endif
