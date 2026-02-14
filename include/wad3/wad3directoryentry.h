#ifndef WAD3DIRECTORYENTRY_H
#define WAD3DIRECTORYENTRY_H

#include <stdint.h>
#include <stdio.h>

#define MAX_TEXTURE_NAME 16

typedef struct {
	int32_t entry_offset; // Offset from the beginning of the WAD3 data
    int32_t disk_size;    // The entry's size in the archive in bytes
    int32_t entry_size;   // The entry's uncompressed size
    char file_type;       // File type of the entry
    int8_t compressed;    // Whether the file was compressed
    int16_t padding;
    char texture_name[MAX_TEXTURE_NAME]; // Null-terminated texture name
} WAD3DirectoryEntry;

WAD3DirectoryEntry * new_wad3directoryentry();
int init_wad3directoryentry(WAD3DirectoryEntry * d);
int init_wad3directoryentry_from_file(WAD3DirectoryEntry * d, FILE * f);
int print_wad3directoryentry(const WAD3DirectoryEntry * d);
void free_wad3directoryentry(WAD3DirectoryEntry * d);

#endif
