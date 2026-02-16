#include "wad3/wad3pic.h"

#include "ifi_errors.h"

int init_wad3pic_from_data(WAD3Pic * p, const uint8_t * data) {
    if (p == NULL || data == NULL) {
        return IFI_ERROR_NULL_ARGS;
    }
    p->width  = unsafe_read_u32le_from_ptr(data + 0);
    p->height = unsafe_read_u32le_from_ptr(data + 4);
    p->indices = (uint8_t *)(data + 8);
    size_t pixel_count = p->width * p->height;
    const uint8_t * palette_start = data + 8 + pixel_count;
    p->palette_size = unsafe_read_u16le_from_ptr(palette_start);
    p->rgb_data = (uint8_t *)(palette_start + 2);
    return IFI_OK;
}

void print_wad3pic(WAD3Pic * p) {
    printf("\n");
    printf("width: %d\n", p->width);
    printf("height: %d\n", p->height);
    for (size_t i = 0; i < (p->width * p->height); i += 1) {
        printf("indices[%zu]: %d\n", i, p->indices[i]);
    }
    printf("palette_size: %d\n", p->palette_size);
    for (size_t i = 0; i < (3 * p->palette_size); i += 1) {
        printf("indices[%zu]: %d\n", i, p->rgb_data[i]);
    }
}
