#include "file_mapper.h"
#include <stdio.h>

#ifdef _WIN32

bool map_file(MappedFile * f, const char * filepath) {
    if (!f) { return false; }
    f->data = NULL;
    f->size = 0;

    f->file_handle = CreateFileA(
        filepath, GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (f->file_handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to open file on Windows.\n");
        return false;
    }

    LARGE_INTEGER file_size;
    if (!GetFileSizeEx(f->file_handle, &file_size)) {
        CloseHandle(f->file_handle);
        return false;
    }
    f->size = (size_t)file_size.QuadPart;

    f->map_handle = CreateFileMappingA(
        f->file_handle, NULL, PAGE_READONLY, 0, 0, NULL);
    if (f->map_handle == NULL) {
        CloseHandle(f->file_handle);
        return false;
    }

    f->data = (uint8_t *)MapViewOfFile(f->map_handle, FILE_MAP_READ, 0, 0, 0);
    if (f->data == NULL) {
        CloseHandle(f->map_handle);
        CloseHandle(f->file_handle);
        return false;
    }

    return true;
}

void unmap_file(MappedFile * f) {
    if (f && f->data) {
        UnmapViewOfFile(f->data);
        CloseHandle(f->map_handle);
        CloseHandle(f->file_handle);
        f->data = NULL;
    }
}

#else

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

bool map_file(MappedFile * f, const char * filepath) {
    if (!f) { return false; }
    f->data = NULL;
    f->size = 0;

    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file");
        return false;
    }

    struct stat file_stat;
    if (fstat(fd, &file_stat) < 0) {
        perror("Failed to get file stats");
        close(fd);
        return false;
    }
    f->size = file_stat.st_size;

    f->data = (uint8_t *)mmap(NULL, f->size, PROT_READ, MAP_PRIVATE, fd, 0);
    
    close(fd);

    if (f->data == MAP_FAILED) {
        perror("Failed to map file to memory");
        f->data = NULL;
        return false;
    }

    return true;
}

void unmap_file(MappedFile * f) {
    if (f && f->data) {
        munmap(f->data, f->size);
        f->data = NULL;
    }
}

#endif
