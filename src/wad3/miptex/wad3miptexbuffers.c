#include "wad3/miptex/wad3miptex.h"
#include "wad3/miptex/wad3miptexbuffers.h"

#include <stdio.h>

#include "utils.h"
#include "ifi_errors.h"

int init_wad3miptexbuffers_from_data(
    WAD3MipTexBuffers * b,
    WAD3MipTex * m,
    const uint8_t * data
) {
    if (b == NULL || m == NULL || data == NULL) {
        return 1;
    }
    b->mipmap_zero  = (uint8_t *)(data + m->offsets[0]);
    b->mipmap_one   = (uint8_t *)(data + m->offsets[1]);
    b->mipmap_two   = (uint8_t *)(data + m->offsets[2]);
    b->mipmap_three = (uint8_t *)(data + m->offsets[3]);
    b->mip0_size = m->width * m->height;
    b->mip1_size = (m->width / 2) * (m->height / 2);
    b->mip2_size = (m->width / 4) * (m->height / 4);
    b->mip3_size = (m->width / 8) * (m->height / 8);
    b->width = m->width;
    b->height = m->height;
    const uint8_t * palette_start = data + m->offsets[3] + b->mip3_size;
    b->palette_size = unsafe_read_u16le_from_ptr(palette_start);
    b->rgb_data = (uint8_t *)(palette_start + 2);
    return 0;
}

