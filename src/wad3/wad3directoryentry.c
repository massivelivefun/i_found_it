#include "wad3/wad3directoryentry.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils/endian_utils.h"
#include "utils/file_utils.h"
#include "ifi_errors.h"

WAD3DirectoryEntry * new_wad3directoryentry() {
    WAD3DirectoryEntry * d =
    	(WAD3DirectoryEntry *)malloc(sizeof(WAD3DirectoryEntry));
    if (d == NULL) {
        return NULL;
    }
    // `d` is valid at this line
    (void)init_wad3directoryentry(d);
    return d;
}

int init_wad3directoryentry(WAD3DirectoryEntry * d) {
    if (d == NULL) { return IFI_ERROR_NULL_ARGS; }
    // Valid because WAD3DirectoryEntry is the owner of all its data
    (void)memset(d, 0, sizeof(WAD3DirectoryEntry));
    return IFI_OK;
}

int init_wad3directoryentry_from_file(WAD3DirectoryEntry * d, FILE * f) {
	if (d == NULL || f == NULL) { return IFI_ERROR_NULL_ARGS; }
	if (read_i32le_from_file(f, &d->entry_offset)) { return IFI_ERROR_READ; }
	if (read_i32le_from_file(f, &d->disk_size)) { return IFI_ERROR_READ; }
	if (read_i32le_from_file(f, &d->entry_size)) { return IFI_ERROR_READ; }
	if (read_char_from_file(f, &d->file_type)) { return IFI_ERROR_READ; }
	if (read_i8_from_file(f, &d->compressed)) { return IFI_ERROR_READ; }
	// Skip padding field
	if (fseek(f, sizeof(int16_t), SEEK_CUR) != 0) { return IFI_ERROR_SEEK; }
	if (read_fixed_string_from_file(f, d->texture_name, MAX_TEXTURE_NAME)) {
		return IFI_ERROR_READ;
	}
	return IFI_OK;
}

int print_wad3directoryentry(const WAD3DirectoryEntry * d) {
	if (d == NULL) { return IFI_ERROR_NULL_ARGS; }
	printf("\nentry_offset: %d\ndisk_size: %d\nentry_size: %d\n"
		"file_type: %c\ncompressed: %d\ntexture_name: %.16s\n",
		d->entry_offset, d->disk_size, d->entry_size, d->file_type,
		d->compressed, d->texture_name);
	return IFI_OK;
}

void free_wad3directoryentry(WAD3DirectoryEntry * d) {
	if (d == NULL) { return; }
	free(d);
}
