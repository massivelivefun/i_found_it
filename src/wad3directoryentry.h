#ifndef WAD3DIRECTORYENTRY_H
#define WAD3DIRECTORYENTRY_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	int32_t entry_offset;  // Offset from the beginning of the WAD3 data
    int32_t disk_size;     // The entry's size in the archive in bytes
    int32_t entry_size;    // The entry's uncompressed size
    char file_type;        // File type of the entry
    _Bool compressed;      // Whether the file was compressed
    int16_t padding;
    char texture_name[16]; // Null-terminated texture name
} WAD3DirectoryEntry;

int new_wad3directoryentry(FILE * f, WAD3DirectoryEntry * d);
void print_wad3directoryentry(WAD3DirectoryEntry * d);

#endif
