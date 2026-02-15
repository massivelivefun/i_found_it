#ifndef WAD3MIPTEXCOLORDATAPALETTE_H
#define WAD3MIPTEXCOLORDATAPALETTE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint16_t palette_size;
    uint8_t * rgb_data;
} WAD3MipTexPaletteColorData;

int init_wad3miptexpalettecolordata_from_data(
    WAD3MipTexPaletteColorData * c,
    const uint8_t * data
);
int print_wad3miptexpalettecolordata(const WAD3MipTexPaletteColorData * c);

#endif
