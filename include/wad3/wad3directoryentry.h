#ifndef WAD3DIRECTORYENTRY_H
#define WAD3DIRECTORYENTRY_H

#include <stdint.h>
#include <stdio.h>
#include "utils/arena.h"

#define MAX_TEXTURE_NAME 16

typedef struct {
	uint32_t entry_offset; // Offset from the beginning of the WAD3 data
    uint32_t disk_size;    // The entry's size in the archive in bytes
    uint32_t entry_size;   // The entry's uncompressed size
    uint8_t file_type;     // File type of the entry
    uint8_t compressed;    // Whether the file was compressed
    uint16_t padding;
    char texture_name[MAX_TEXTURE_NAME]; // Null-terminated texture name
} WAD3DirectoryEntry;

int init_wad3directoryentry_from_data(
    WAD3DirectoryEntry * d, const int * entry_data
);
WAD3DirectoryEntry * parse_directories_from_data(
    Arena * a, const uint8_t * file_data, uint32_t dir_offset, uint32_t num_dirs
);
int print_wad3directoryentry(const WAD3DirectoryEntry * d);

#endif
