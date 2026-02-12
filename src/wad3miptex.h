#ifndef WAD3MIPTEX_H
#define WAD3MIPTEX_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_TEXTURE_NAME 16
#define MAX_OFFSETS 4

typedef struct {
    char name[MAX_TEXTURE_NAME];   // Name of the texture (same as dir_entry)
    uint32_t width;                // Width of image in pixels
    uint32_t height;               // Height of image in pixels
    uint32_t offsets[MAX_OFFSETS]; // Offsets to the 4 mipmap levels
} WAD3MipTex;

typedef struct {
    uint8_t * mipmap_zero;
    uint8_t * mipmap_one;
    uint8_t * mipmap_two;
    uint8_t * mipmap_three;
    size_t mipmap_zero_size;
    size_t mipmap_one_size;
    size_t mipmap_two_size;
    size_t mipmap_three_size;
} WAD3MipTexBuffers;

typedef struct {
    uint16_t palette_size;
    uint8_t * rgb_data;
} WAD3MipTexPaletteColorData;

int new_wad3miptex_from_file(WAD3MipTex * m, FILE * f);
int print_wad3miptex(const WAD3MipTex * m);

int new_wad3miptexbuffers(FILE * f, WAD3MipTex * m, WAD3MipTexBuffers * b, int32_t entry_offset);

int new_wad3miptexbuf(FILE * f, size_t mipmap_size, uint8_t * mipmap_ptr, uint32_t offset, int32_t entry_offset);

int new_alloc_wad3miptexbuffers_from_file(
    WAD3MipTexBuffers * b,
    FILE * f,
    WAD3MipTex * m,
    int32_t entry_offset
);
int free_alloc_wad3miptexbuffers(WAD3MipTexBuffers * b);

int new_wad3miptexpalettecolordata(FILE * f, WAD3MipTexPaletteColorData * c);

int new_alloc_wad3miptexpalettecolordata_from_file(WAD3MipTexPaletteColorData * c, FILE * f);

int set_wad3miptexpalettecolordata_palette_size(FILE * f, WAD3MipTexPaletteColorData * c);
int set_wad3miptexpalettecolordata_rgb_data(FILE * f, WAD3MipTexPaletteColorData * c);
int print_wad3miptexpalettecolordata(const WAD3MipTexPaletteColorData * c);
int free_wad3miptexpalettecolordata(WAD3MipTexPaletteColorData * c);

#endif
