#include <stdbool.h>
#include <stdio.h>

#include "wad3directoryentry.h"

int new_wad3directoryentry(FILE * f, WAD3DirectoryEntry * d) {
	if (f == NULL || d == NULL) {
		return 1;
	}
	if (fread(&(d->entry_offset), sizeof(int), 1, f) < 1) {
		return 1;
	}
	if (fread(&(d->disk_size), sizeof(int), 1, f) < 1) {
		return 1;
	}
	if (fread(&(d->entry_size), sizeof(int), 1, f) < 1) {
		return 1;
	}
	if (fread(&(d->file_type), sizeof(char), 1, f) < 1) {
		return 1;
	}
	if (fread(&(d->compressed), sizeof(_Bool), 1, f) < 1) {
		return 1;
	}
	fseek(f, sizeof(short), SEEK_CUR);
	if (fread(&(d->texture_name), sizeof(char), 15, f) < 15) {
		return 1;
	}
	fseek(f, sizeof(char), SEEK_CUR);
	d->texture_name[15] = '\0';
	return 0;
}

void print_wad3directoryentry(WAD3DirectoryEntry * d) {
	printf("\n");
	printf("entry_offset: %d\n", d->entry_offset);
	printf("disk_size: %d\n", d->disk_size);
	printf("entry_size: %d\n", d->entry_size);
	printf("file_type: %c\n", d->file_type);
	printf("compressed: %d\n", d->compressed);
	// printf("padding: %d\n", d->padding);
	printf("texture_name: %s\n", d->texture_name);
}
