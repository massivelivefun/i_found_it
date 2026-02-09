#ifndef WAD3FONT_H
#define WAD3FONT_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint16_t start_offset; // Horizontal offset in the texture
    uint16_t char_width;   // Width of this specific character
} WAD3FontCharInfo;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t row_count;
    uint32_t row_height;
    WAD3FontCharInfo char_info[256];
    uint8_t * indices;
    uint16_t palette_size;
    uint8_t * rgb_data;
} WAD3Font;

#endif
