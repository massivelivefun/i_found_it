#ifndef WAD3HEADER_H
#define WAD3HEADER_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
	char magic[4];
	uint32_t num_dirs;
	uint32_t dir_offset;
} WAD3Header;

int new_wad3header_from_file(WAD3Header * h, FILE * f);
void print_wad3header(const WAD3Header * h);

#endif
