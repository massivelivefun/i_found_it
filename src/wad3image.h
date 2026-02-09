#ifndef WAD3IMAGE_H
#define WAD3IMAGE_H

#include <string.h>

#include "wad3miptex.h"
#include "wad3pic.h"

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t *indices;
    uint8_t *rgb_data;
    char name[16];
} WAD3Image;

int new_wad3image(
    WAD3MipTex * m,
    uint8_t * indices,
    uint8_t * rgb_data,
    uint8_t dimension,
    WAD3Image * i
);

int new_wad3image_from_wad3pic(
    WAD3Pic * pic,
    char * name,
    uint8_t dimension,
    WAD3Image * i
);

#endif
