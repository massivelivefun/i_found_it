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

//
// WAD3MipTex functions
//

WAD3MipTex * new_wad3miptex();
int init_wad3miptex(WAD3MipTex * m);
int init_wad3miptex_from_file(WAD3MipTex * m, FILE * f);
int print_wad3miptex(const WAD3MipTex * m);
void free_wad3miptex(WAD3MipTex * m);

#endif
