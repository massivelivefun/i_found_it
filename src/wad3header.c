#include <stdio.h>

#include "wad3header.h"

int new_wad3header(FILE * f, WAD3Header * h) {
	if (f == NULL || h == NULL) {
		return 1;
	}
	if (fread(h->magic, sizeof(char), 4, f) < 4) {
		return 1;
	}
	if (fread(&(h->num_dirs), sizeof(int), 1, f) < 1) {
		return 1;
	}
	if (fread(&(h->dir_offset), sizeof(int), 1, f) < 1) {
		return 1;
	}
	return 0;
}

void print_wad3header(WAD3Header * h) {
	printf("\n");
	printf("magic: ");
	fwrite(h->magic, sizeof(char), 4, stdout);
	printf("\n");
	printf("num_dirs: %d\n", h->num_dirs);
	printf("dir_offset: %d\n", h->dir_offset);
}
