#include "wad3/miptex/wad3miptexpalettecolordata.h"

#include <stdio.h>

#include "utils.h"
#include "ifi_errors.h"

int init_wad3miptexpalettecolordata_from_data(
    WAD3MipTexPaletteColorData * c,
    const uint8_t * data
) {
    if (c == NULL) { return IFI_ERROR_NULL_ARGS; }
    c->palette_size = unsafe_read_u16le_from_ptr(data);
    c->rgb_data = (uint8_t *)(data + 2);
    return IFI_OK;
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
