#ifndef WAD3MIPTEXBUFFERS_H
#define WAD3MIPTEXBUFFERS_H

#include "wad3miptex.h"

#include <stdio.h>

#include "utils/endian_utils.h"
#include "utils/file_utils.h"
#include "ifi_errors.h"

typedef struct {
    uint8_t * mipmap_zero;
    uint8_t * mipmap_one;
    uint8_t * mipmap_two;
    uint8_t * mipmap_three;
    size_t mip0_size;
    size_t mip1_size;
    size_t mip2_size;
    size_t mip3_size;
} WAD3MipTexBuffers;

int init_wad3miptexbuffers_from_data(
    WAD3MipTexBuffers * b, WAD3MipTex * m, const uint8_t * data
);

#endif
