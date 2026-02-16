#ifndef WAD3FONT_H
#define WAD3FONT_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint16_t start_offset; // Horizontal offset in the texture
    uint16_t char_width;   // Width of this specific character
} WAD3FontCharInfo;

typedef struct {
    uint32_t width;        // Width is always 256 pixels
    uint32_t height;       // So probably height is 256 pixels also
    uint32_t row_count;    // Number of fixed-height character rows
    uint32_t row_height;   // Height of a row
    WAD3FontCharInfo char_info[256]; // Info about each character
    uint8_t * indices;     // Width * Height
    uint16_t palette_size; // Colors
    uint8_t * rgb_data;    // Palette_size * 3
} WAD3Font;

int init_wad3font_from_data(WAD3Font * f, const uint8_t * data);

#endif
