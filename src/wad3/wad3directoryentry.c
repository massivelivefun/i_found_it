#include "wad3/wad3directoryentry.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "ifi_errors.h"

int init_wad3directoryentry_from_data(
	WAD3DirectoryEntry * d, const int * entry_data
) {
	if (d == NULL || entry_data == NULL) { return IFI_ERROR_NULL_ARGS; }
	d->entry_offset = unsafe_read_u32le_from_ptr(entry_data + 0);
	d->disk_size = unsafe_read_u32le_from_ptr(entry_data + 4);
	d->entry_size = unsafe_read_u32le_from_ptr(entry_data + 8);
	d->file_type = entry_data[12];
	d->compressed = entry_data[13];
	d->padding = unsafe_read_u16le_from_ptr(entry_data + 14);
	memcpy(d->texture_name, entry_data + 16, 16);
	return IFI_OK;
}

WAD3DirectoryEntry * parse_directories_from_data(
	Arena * a,
	const uint8_t * file_data,
	uint32_t dir_offset,
	uint32_t num_dirs
) {
    WAD3DirectoryEntry * entries =
        arena_push(a, sizeof(WAD3DirectoryEntry) * num_dirs);
    if (entries == NULL) { return NULL; }
    const uint8_t * dir_start = file_data + dir_offset;
    for (size_t i = 0; i < num_dirs; i++) {
    	const uint8_t * curr_entry_data = dir_start +
    		(i * sizeof(WAD3DirectoryEntry));
        init_wad3directoryentry_from_data(&entries[i], curr_entry_data);
    }
    return entries;
}

int print_wad3directoryentry(const WAD3DirectoryEntry * d) {
	if (d == NULL) { return IFI_ERROR_NULL_ARGS; }
	printf("\nentry_offset: %d\ndisk_size: %d\nentry_size: %d\n"
		"file_type: %c\ncompressed: %d\ntexture_name: %.16s\n",
		d->entry_offset, d->disk_size, d->entry_size, d->file_type,
		d->compressed, d->texture_name);
	return IFI_OK;
}
