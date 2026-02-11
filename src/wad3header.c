#include "wad3header.h"

#include "endian_utils.h"
#include "ifi_errors.h"

int new_wad3header_from_file(WAD3Header * h, FILE * f) {
	if (h == NULL || f == NULL) { return IFI_ERROR_NULL_ARGS; }
	if (fread(h->magic, sizeof(char), 4, f) < 4) { return IFI_ERROR_READ; }
	if (read_u32le_from_file(f, &h->num_dirs)) { return IFI_ERROR_READ; }
	if (read_u32le_from_file(f, &h->dir_offset)) { return IFI_ERROR_READ; }
	return IFI_OK;
}

void print_wad3header(const WAD3Header * h) {
	if (h == NULL) { return; }
	// %.4s is needed, magic is not a null terminating string
	printf("\nmagic: %.4s\nnum_dirs: %u\ndir_offset: %u\n",
    	h->magic, h->num_dirs, h->dir_offset);
}
