#ifndef WAD3HEADER_H
#define WAD3HEADER_H

#include <stdint.h>

typedef struct {
	char magic[4];
	uint32_t num_dirs;
	uint32_t dir_offset;
} WAD3Header;

int new_wad3header(FILE * f, WAD3Header * h);
void print_wad3header(WAD3Header * h);

#endif
