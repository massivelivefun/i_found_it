#include "wad3/miptex/wad3miptexpalettecolordata.h"

#include <stdio.h>

#include "utils/endian_utils.h"
#include "utils/file_utils.h"
#include "ifi_errors.h"

//
// Palette Color Data
//

WAD3MipTexPaletteColorData * new_wad3miptexpalettecolordata() {
    WAD3MipTexPaletteColorData * c = (WAD3MipTexPaletteColorData *)
        malloc(sizeof(WAD3MipTexPaletteColorData));
    if (c == NULL) { return NULL; }
    c->palette_size = 0;
    c->rgb_data = NULL;
    return c;
}

int init_wad3miptexpalettecolordata(
    WAD3MipTexPaletteColorData * c,
    uint8_t * buffer,
    uint16_t size
) {
    if (c == NULL) { return IFI_ERROR_NULL_ARGS; }
    c->palette_size = size;
    c->rgb_data = buffer;
    return IFI_OK;
}

// DO NOT CALL THIS WITH A WAD3MIPTEXPALETTECOLORDATA HOLDING A BUFFER ON THE
// STACK, IT WILL TRY TO FREE A STACK POINTER, WHICH IS BEYOND WRONG
int init_owning_wad3miptexpalettecolordata(
    WAD3MipTexPaletteColorData * c,
    uint16_t size
) {
    if (c == NULL) { return IFI_ERROR_NULL_ARGS; }
    if (c->rgb_data != NULL) { free(c->rgb_data); }
    c->palette_size = size;
    c->rgb_data = (uint8_t *)malloc(sizeof(uint8_t) * (size * 3));
    if (c->rgb_data == NULL) { return IFI_ERROR_MEMORY; }
    return IFI_OK;
}

// DO NOT CALL THIS WITH A WAD3MIPTEXPALETTECOLORDATA HOLDING A BUFFER ON THE
// STACK, IT WILL TRY TO FREE A STACK POINTER, WHICH IS BEYOND WRONG
int init_owning_wad3miptexpalettecolordata_from_file(
    WAD3MipTexPaletteColorData * c,
    FILE * f
) {
    if (c == NULL || f == NULL) {
        fprintf(stderr, "Arguments cannot be NULL.\n");
        return IFI_ERROR_NULL_ARGS;
    }
    int result = IFI_OK;

    if (c->rgb_data != NULL) {
        free(c->rgb_data);
    }
    c->palette_size = 0;
    c->rgb_data = NULL;

    if (read_u16le_from_file(f, &c->palette_size)) {
        fprintf(stderr, "Failed to read palette size.\n");
        result = IFI_ERROR_READ;
        goto error_cleanup;
    }
    if (c->palette_size == 0) {
        fprintf(stderr, "Palette size is invalid (0).\n");
        result = IFI_ERROR_INVALID;
        goto error_cleanup;
    }
    size_t rgb_data_size = c->palette_size * 3;
    c->rgb_data = (uint8_t *)malloc(sizeof(uint8_t) * rgb_data_size);
    if (c->rgb_data == NULL) {
        fprintf(stderr, "Failed to allocate rgb data.\n");
        result = IFI_ERROR_MEMORY;
        goto error_cleanup;
    }
    if (fread(c->rgb_data, sizeof(uint8_t), rgb_data_size, f)
            != rgb_data_size) {
        fprintf(stderr, "Failed to read palette data.\n");
        result = IFI_ERROR_READ;
        goto error_cleanup;
    }
    return result;
    
error_cleanup:
    free_owning_wad3miptexpalettecolordata(c);
    return result;
}

int print_wad3miptexpalettecolordata(const WAD3MipTexPaletteColorData * c) {
    if (c == NULL) { return IFI_ERROR_NULL_ARGS; }
    int len = snprintf(NULL, 0, "\npalette_size: %d\nrgb_data_size: %d\n",
        c->palette_size, c->palette_size * 3);
    if (len < 0) { return IFI_ERROR_ENCODING; }
    size_t total_len = (size_t)len;
    for (size_t i = 0; i < c->palette_size; i += 1) {
        size_t idx = i * 3;
        len = snprintf(NULL, 0, "%d %d %d\n",
            c->rgb_data[idx + 0], c->rgb_data[idx + 1], c->rgb_data[idx + 2]);
        if (len < 0) { return IFI_ERROR_ENCODING; }
        total_len += len;
    }
    char * buffer = (char *)malloc(total_len + 1);
    if (buffer == NULL) { return IFI_ERROR_MEMORY; }
    char * ptr = buffer;
    ptr += sprintf(ptr, "\npalette_size: %d\nrgb_data_size: %d\n",
        c->palette_size, c->palette_size * 3);
    for (size_t i = 0; i < c->palette_size; i += 1) {
        size_t idx = i * 3;
        ptr += sprintf(ptr, "%d %d %d\n",
            c->rgb_data[idx + 0], c->rgb_data[idx + 1], c->rgb_data[idx + 2]);
    }
    printf("%s", buffer);
    free(buffer);
    return IFI_OK;
}

int free_owning_wad3miptexpalettecolordata(WAD3MipTexPaletteColorData * c) {
    if (c == NULL) { return IFI_ERROR_NULL_ARGS; }
    free(c->rgb_data);
    c->palette_size = 0;
    c->rgb_data = NULL;
    return IFI_OK;
}
