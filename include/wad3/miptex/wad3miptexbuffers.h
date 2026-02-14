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
    size_t mipmap_zero_size;
    size_t mipmap_one_size;
    size_t mipmap_two_size;
    size_t mipmap_three_size;
} WAD3MipTexBuffers;

//
// WAD3MipTexBuffers functions
//

int new_wad3miptexbuffers(FILE * f, WAD3MipTex * m, WAD3MipTexBuffers * b,
    int32_t entry_offset);
int new_wad3miptexbuf(FILE * f, size_t mipmap_size, uint8_t * mipmap_ptr,
    uint32_t offset, int32_t entry_offset);
int new_wad3miptexbuffers_from_file(WAD3MipTexBuffers * b, FILE * f,
    WAD3MipTex * m, int32_t entry_offset);
int free_wad3miptexbuffers(WAD3MipTexBuffers * b);

#endif
