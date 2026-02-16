#ifndef WAD3PIC_H
#define WAD3PIC_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/endian_utils.h"

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t * indices;
    uint16_t palette_size;
    uint8_t * rgb_data;
} WAD3Pic;

int init_wad3pic_from_data(WAD3Pic * p, const uint8_t * data);
void print_wad3pic(WAD3Pic * p);

#endif
