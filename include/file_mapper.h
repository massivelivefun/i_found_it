#ifndef FILE_MAPPER_H
#define FILE_MAPPER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
#endif

typedef struct {
    uint8_t * data;
    size_t size;
#ifdef _WIN32
    HANDLE file_handle;
    HANDLE map_handle;
#endif
} MappedFile;

bool map_file(MappedFile * f, const char * filepath);
void unmap_file(MappedFile * f);

#endif
