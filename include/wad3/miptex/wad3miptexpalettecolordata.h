#ifndef WAD3MIPTEXCOLORDATAPALETTE_H
#define WAD3MIPTEXCOLORDATAPALETTE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint16_t palette_size;
    uint8_t * rgb_data;
} WAD3MipTexPaletteColorData;

//
// WAD3MipTexPaletteColorData
//

WAD3MipTexPaletteColorData * new_wad3miptexpalettecolordata();
int init_wad3miptexpalettecolordata(
    WAD3MipTexPaletteColorData * c,
    uint8_t * buffer,
    uint16_t size
);
int init_owning_wad3miptexpalettecolordata(
    WAD3MipTexPaletteColorData * c,
    uint16_t size
);
int init_owning_wad3miptexpalettecolordata_from_file(
    WAD3MipTexPaletteColorData * c,
    FILE * f
);
int print_wad3miptexpalettecolordata(const WAD3MipTexPaletteColorData * c);
int free_owning_wad3miptexpalettecolordata(WAD3MipTexPaletteColorData * c);

#endif
