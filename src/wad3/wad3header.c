#include "wad3/wad3header.h"

#include <stdlib.h>

#include "utils/endian_utils.h"
#include "ifi_errors.h"

WAD3Header * new_wad3header() {
	WAD3Header * h = (WAD3Header *)malloc(sizeof(WAD3Header));
	if (h == NULL) { return NULL; }
	// `h` is valid at this line
	(void)init_wad3header(h);
	return h;
}

int init_wad3header(WAD3Header * h) {
	if (h == NULL) { return IFI_ERROR_NULL_ARGS; }
	// `memset` is valid, WAD3Header is the owner of all its data
	(void)memset(h, 0, sizeof(WAD3Header));
	return IFI_OK;
}

int init_wad3header_from_file(WAD3Header * h, FILE * f) {
	if (h == NULL || f == NULL) { return IFI_ERROR_NULL_ARGS; }
	if (fread(h->magic, sizeof(char), 4, f) < 4) { return IFI_ERROR_READ; }
	if (read_u32le_from_file(f, &h->num_dirs)) { return IFI_ERROR_READ; }
	if (read_u32le_from_file(f, &h->dir_offset)) { return IFI_ERROR_READ; }
	return IFI_OK;
}

int print_wad3header(const WAD3Header * h) {
	if (h == NULL) { return IFI_ERROR_NULL_ARGS; }
	// %.4s is needed, magic is not a null terminating string
	printf("\nmagic: %.4s\nnum_dirs: %u\ndir_offset: %u\n",
    	h->magic, h->num_dirs, h->dir_offset);
	return IFI_OK;
}

void free_wad3header(WAD3Header * h) {
	if (h == NULL) { return; }
	free(h);
}
