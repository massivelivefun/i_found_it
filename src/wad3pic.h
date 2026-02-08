#ifndef WAD3PIC_H
#define WAD3PIC_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t * indices;
    uint16_t palette_size;
    uint8_t * rgb_data;
} WAD3Pic;

int new_wad3pic(FILE * f, WAD3Pic * p);

#endif
