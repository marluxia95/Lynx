#include <errno.h>
#include <stdint.h>
#include <string.h>
#include "package.h"
#include "logger.h"

#define PKG_VER 0

namespace Lynx {

pkg_file_t *g_pkg_head;

bool pkg_read(char* filename)
{
    FILE *fp;
    pkg_header_t header;
    pkg_dir_t dir;
    pkg_file_t *file_node = g_pkg_head;
    int file_count = 0;

    fp = fopen(filename, "rb");
    if(!fp)
        return 1;

    if(!fread(&header, sizeof(header), 1, fp))
        goto error;

    if(memcmp(header.sig, "PKG", 3) != 0)
        goto error;

    file_count = header.size / sizeof(pkg_file_t);

    if(fseek(fp, sizeof(header), SEEK_SET) != 0)
        goto error;

    log_debug("PKG File count %d", file_count);
    for( int i = 0; i < file_count; i++ ) {
        pkg_file_t* file = (pkg_file_t*)malloc(sizeof(pkg_file_t));

        if(!fread(file, sizeof(pkg_file_t) - sizeof(char*), 1, fp))
            goto error;

        log_debug("Loading %s", file->name);

        if(file_node == NULL) {
            file_node = file;
            continue;
        }

        file_node->next = file;
        file_node = file;
    }

    fclose(fp);
    return 0;

error:
    log_error("Failed to open package file %s: %s", filename, strerror(errno));
    fclose(fp);
    return 1;
}

bool pkg_write(char* pkgname, char* filename, void* data, size_t size)
{
    FILE *fp;
    pkg_header_t header;
    int num_files = 0;

    fp = fopen(pkgname, "r+");
    if(!fp){
        log_error("Unable to open file %s: %s", pkgname, strerror(errno));
        return 1;
    }

    // Create a header if the file is new
    if(!fread(&header, sizeof(header), 1, fp)) {
        memcpy(header.sig, "PKG", 3);
        header.version = PKG_VER;
        fwrite(&header, sizeof(header), 1, fp);
        log_debug("Creating PKG header");
    }

    log_debug("PKG size : %d", header.size);

    if(fseek(fp, sizeof(header), SEEK_SET) != 0) {
        log_error("Error while reading file %s: %s", pkgname, strerror(errno));
        return 1;
    }

    int file_count = header.size / sizeof(pkg_file_t);
    log_debug("File count %d", file_count);

    for(int i = 0; i < file_count; i++) {
        pkg_file_t file;

        if(!fread(&file, sizeof(file) - sizeof(char*), 1, fp)) {
            log_error("Error while reading file : %s", strerror(errno));
            return 1;
        }

        if(!strcmp(file.name, filename)) {
            log_debug("File has identical names");
            return 0;
        }

        fseek(fp, file.size, SEEK_CUR);
    }

    pkg_file_t new_file;
    strncpy(new_file.name, filename, 512);
    new_file.size = 0;
    new_file.offset = sizeof(new_file);
    fwrite(&new_file, sizeof(new_file), 1, fp);
    fwrite(data, size, 1, fp);

    rewind(fp);
    header.size += sizeof(pkg_file_t);
    fwrite(&header, sizeof(header), 1, fp);
    log_debug("PKG size : %d", header.size);

    fclose(fp);
    return 0;
}

void pkg_dump(char* pkgname)
{
    FILE *fp;
    pkg_header_t header;
    int file_count;
    pkg_file_t file;

    fp = fopen(pkgname, "rb");
    if(!fp)
        goto dumperror;
    
    if(!fread(&header, sizeof(header), 1, fp)) 
        goto dumperror;

    if(!memcmp(header.sig, "PKG", 3))
        log_warn("PKGDUMP : Header signature mismatch");

    log_debug("PKGDUMP : Header file version %d", header.version);
    log_debug("PKGDUMP : Header file size %d bytes", header.size);

    file_count = header.size / sizeof(pkg_file_t);
    log_debug("PKGDUMP : Size    Offset    Name");
    for(int i = 0; i < file_count; i++) {
        if(!fread(&file, sizeof(file) - sizeof(char*), 1, fp))
            goto dumperror;

        log_debug("PKGDUMP : %d %d %s", file.size, file.offset, file.name);

        fseek(fp, file.size, SEEK_CUR);
    }

    log_debug("PKGDUMP : End");
    fclose(fp);
    return;

dumperror:
    log_error("PKGDUMP : %s", strerror(errno));
    fclose(fp);
}

}
