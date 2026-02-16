#ifndef WAD3HEADER_H
#define WAD3HEADER_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    char magic[4];
    uint32_t num_dirs;
    uint32_t dir_offset;
} WAD3Header;

int init_wad3header_from_data(WAD3Header * h, const uint8_t * data);
int print_wad3header(const WAD3Header * h);
int validate_wad3header_magic(const WAD3Header * h);

#endif
