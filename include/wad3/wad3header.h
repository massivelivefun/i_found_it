#ifndef WAD3HEADER_H
#define WAD3HEADER_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
	char magic[4];
	uint32_t num_dirs;
	uint32_t dir_offset;
} WAD3Header;

WAD3Header * new_wad3header();
int init_wad3header(WAD3Header * h);
int init_wad3header_from_file(WAD3Header * h, FILE * f);
int print_wad3header(const WAD3Header * h);
void free_wad3header(WAD3Header * h);

#endif
