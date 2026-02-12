#include "wad3directoryentry.h"

#include <stdbool.h>
#include <stdio.h>

#include "endian_utils.h"
#include "file_utils.h"
#include "ifi_errors.h"

int new_wad3directoryentry_from_file(WAD3DirectoryEntry * d, FILE * f) {
	if (d == NULL || f == NULL) { return IFI_ERROR_NULL_ARGS; }
	if (read_i32le_from_file(f, &d->entry_offset)) { return IFI_ERROR_READ; }
	if (read_i32le_from_file(f, &d->disk_size)) { return IFI_ERROR_READ; }
	if (read_i32le_from_file(f, &d->entry_size)) { return IFI_ERROR_READ; }
	if (read_char_from_file(f, &d->file_type)) { return IFI_ERROR_READ; }
	if (read_i8_from_file(f, &d->compressed)) { return IFI_ERROR_READ; }
	// Skip padding field
	if (fseek(f, sizeof(int16_t), SEEK_CUR) != 0) { return IFI_ERROR_READ; }
	if (read_fixed_string_from_file(f, d->texture_name, MAX_TEXTURE_NAME)) {
		return IFI_ERROR_READ;
	}
	return 0;
}

void print_wad3directoryentry(const WAD3DirectoryEntry * d) {
	if (d == NULL) { return; }
	printf("\nentry_offset: %d\ndisk_size: %d\nentry_size: %d\nfile_type: "
		"%c\ncompressed: %d\ntexture_name: %.16s\n", d->entry_offset,
		d->disk_size, d->entry_size, d->file_type, d->compressed,
		d->texture_name);
}
