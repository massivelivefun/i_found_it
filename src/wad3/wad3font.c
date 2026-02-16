#include "wad3/wad3font.h"

#include "ifi_errors.h"

#include "utils/endian_utils.h"

int init_wad3font_from_data(WAD3Font * f, const uint8_t * data) {
    if (f == NULL || data == NULL) {
        return IFI_ERROR_NULL_ARGS;
    }
    f->width = unsafe_read_u32le_from_ptr(data + 0);
    f->height = unsafe_read_u32le_from_ptr(data + 4);
    f->row_count = unsafe_read_u32le_from_ptr(data + 8);
    f->row_height = unsafe_read_u32le_from_ptr(data + 12);
    for (size_t i = 0; i < 256; i += 1) {
        size_t char_idx = 16 + (i * 4);
        f->char_info[i].start_offset = unsafe_read_u16le_from_ptr(
            data + char_idx);
        f->char_info[i].char_width = unsafe_read_u16le_from_ptr(
            data + char_idx + 2);
    }
    const uint8_t * indices_start = (uint8_t *)(data + 16 + (4 * 256));
    f->indices = (uint8_t *)indices_start;
    const uint8_t * palette_start = indices_start + (f->width * f->height);
    f->palette_size = unsafe_read_u16le_from_ptr(palette_start);
    f->rgb_data = (uint8_t *)(palette_start + 2);
    return IFI_OK;
}
