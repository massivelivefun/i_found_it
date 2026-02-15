#ifndef WAD3MIPTEX_H
#define WAD3MIPTEX_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_TEXTURE_NAME 16
#define MAX_OFFSETS 4

typedef struct {
    char name[MAX_TEXTURE_NAME];   // Name of the texture (same as dir_entry) this is null terminated
    uint32_t width;                // Width of image in pixels
    uint32_t height;               // Height of image in pixels
    uint32_t offsets[MAX_OFFSETS]; // Offsets to the 4 mipmap levels
} WAD3MipTex;

//
// WAD3MipTex functions
//

int init_wad3miptex(WAD3MipTex * m);
int init_wad3miptex_from_data(WAD3MipTex * m, const uint8_t * data);
int print_wad3miptex(const WAD3MipTex * m);

#endif
