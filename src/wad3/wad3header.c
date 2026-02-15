#include "wad3/wad3header.h"

#include <stdlib.h>

#include "utils/endian_utils.h"
#include "ifi_errors.h"

#define MAKE_FOURCC(a, b, c, d) \
    ((uint32_t)(a) | ((uint32_t)(b) << 8) | \
    ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))

int init_wad3header_from_data(WAD3Header * h, const uint8_t * data) {
    if (h == NULL || data == NULL) { return IFI_ERROR_NULL_ARGS; }
    memcpy(h->magic, data, 4);
    h->num_dirs = unsafe_read_u32le_from_ptr(data + 4);
    h->dir_offset = unsafe_read_u32le_from_ptr(data + 8);
    return IFI_OK;
}

int print_wad3header(const WAD3Header * h) {
    if (h == NULL) { return IFI_ERROR_NULL_ARGS; }
    // %.4s is needed, magic is not a null terminating string
    printf("\nmagic: %.4s\nnum_dirs: %u\ndir_offset: %u\n",
        h->magic, h->num_dirs, h->dir_offset);
    return IFI_OK;
}

int validate_wad3header_magic(const WAD3Header * h) {
    uint32_t magic = *(uint32_t *)(h->magic);

    switch (magic) {
        case MAKE_FOURCC('W', 'A', 'D', '3'):
            return IFI_OK;
        case MAKE_FOURCC('W', 'A', 'D', '2'):
            fprintf(stderr, "WAD2 not supported.\n");
            return 1;
        case MAKE_FOURCC('W', 'A', 'D', 'F'):
            fprintf(stderr, "WADF not supported.\n");
            return 1;
        case MAKE_FOURCC('B', 'S', 'P', 'X'):
            fprintf(stderr, "BSPX not supported.\n");
            return 1;
        case MAKE_FOURCC('I', 'W', 'A', 'D'):
            fprintf(stderr, "IWAD not supported.\n");
            return 1;
        case MAKE_FOURCC('P', 'W', 'A', 'D'):
            fprintf(stderr, "PWAD not supported.\n");
            return 1;
        default:
            fprintf(stderr, "Magic not recognized: %.4s\n", h->magic);
            return 1;
    }
}
