#include "wad3/miptex/wad3miptex.h"
#include "wad3/miptex/wad3miptexbuffers.h"

#include <stdio.h>

#include "utils/endian_utils.h"
#include "utils/file_utils.h"
#include "ifi_errors.h"

//
// Mipmap Texture Buffers
//

int new_wad3miptexbuffers_from_file(
    WAD3MipTexBuffers * b,
    FILE * f,
    WAD3MipTex * m,
    int32_t entry_offset
) {
    if (b == NULL || f == NULL || m == NULL) {
        return 1;
    }

    // This make it safe to call free_alloc_wad3miptexbuffers at anytime
    b->mipmap_zero = NULL;
    b->mipmap_one = NULL;
    b->mipmap_two = NULL;
    b->mipmap_three = NULL;

    b->mipmap_zero_size = 0;
    b->mipmap_one_size = 0;
    b->mipmap_two_size = 0;
    b->mipmap_three_size = 0;

    size_t size_zero = m->width * m->height;
    size_t size_one = (m->width / 2) * (m->height / 2);
    size_t size_two = (m->width / 4) * (m->height / 4);
    size_t size_three = (m->width / 8) * (m->height / 8);

    b->mipmap_zero = (uint8_t *)malloc(sizeof(uint8_t) * size_zero);
    if (b->mipmap_zero == NULL) {
        fprintf(stderr, "Failed to allocate mipmap_zero.\n");
        goto error_memory;
    }
    b->mipmap_zero_size = size_zero;

    b->mipmap_one = (uint8_t *)malloc(sizeof(uint8_t) * size_one);
    if (b->mipmap_one == NULL) {
        fprintf(stderr, "Failed to allocate mipmap_one.\n");
        goto error_memory;
    }
    b->mipmap_one_size = size_one;

    b->mipmap_two = (uint8_t *)malloc(sizeof(uint8_t) * size_two);
    if (b->mipmap_two == NULL) {
        fprintf(stderr, "Failed to allocate mipmap_two.\n");
        goto error_memory;
    }
    b->mipmap_two_size = size_two;

    b->mipmap_three = (uint8_t *)malloc(sizeof(uint8_t) * size_three);
    if (b->mipmap_three == NULL) {
        fprintf(stderr, "Failed to allocate mipmap_three.\n");
        goto error_memory;
    }
    b->mipmap_three_size = size_three;

    int result = new_wad3miptexbuffers(f, m, b, entry_offset);
    if (result != IFI_OK) {
        fprintf(stderr, "Failed to fill buffers with data.\n");
        goto error_cleanup;
    }

    return IFI_OK;

error_memory:
    result = IFI_ERROR_MEMORY;
error_cleanup:
    free_wad3miptexbuffers(b);
    return result;
}

int new_wad3miptexbuffers(
    FILE * f,
    WAD3MipTex * m,
    WAD3MipTexBuffers * b,
    int32_t entry_offset
) {
    if (f == NULL || m == NULL || b == NULL) {
        return 1;
    }
    if (new_wad3miptexbuf(f, b->mipmap_zero_size, b->mipmap_zero,
            m->offsets[0], entry_offset)) {
        fprintf(stderr, "Failed to read mipmap_zero data.\n");
        return 1;
    }
    if (new_wad3miptexbuf(f, b->mipmap_one_size, b->mipmap_one,
            m->offsets[1], entry_offset)) {
        fprintf(stderr, "Failed to read mipmap_one data.\n");
        return 1;
    }
    if (new_wad3miptexbuf(f, b->mipmap_two_size, b->mipmap_two,
            m->offsets[2], entry_offset)) {
        fprintf(stderr, "Failed to read mipmap_two data.\n");
        return 1;
    }
    if (new_wad3miptexbuf(f, b->mipmap_three_size, b->mipmap_three,
            m->offsets[3], entry_offset)) {
        fprintf(stderr, "Failed to read mipmap_three data.\n");
        return 1;
    }
    return 0;
}

int new_wad3miptexbuf(
    FILE * f,
    size_t mipmap_size,
    uint8_t * mipmap_ptr,
    uint32_t offset,
    int32_t entry_offset
) {
    if (f == NULL || mipmap_ptr == NULL) {
        return 1;
    }
    if(fseek(f, entry_offset + offset, SEEK_SET) != 0) {
        fprintf(stderr, "Failed to seek to mipmap data offset.\n");
        return 1;
    }
    if (fread(mipmap_ptr, sizeof(uint8_t), mipmap_size, f) < mipmap_size) {
        fprintf(stderr, "Failed to read full mipmap from file.\n");
        return 1;
    }
    return 0;
}

int free_wad3miptexbuffers(WAD3MipTexBuffers * b) {
    if (b == NULL) {
        return IFI_ERROR_NULL_ARGS;
    }

    free(b->mipmap_zero);
    b->mipmap_zero = NULL;
    b->mipmap_zero_size = 0;

    free(b->mipmap_one);
    b->mipmap_one = NULL;
    b->mipmap_one_size = 0;

    free(b->mipmap_two);
    b->mipmap_two = NULL;
    b->mipmap_two_size = 0;

    free(b->mipmap_three);
    b->mipmap_three = NULL;
    b->mipmap_three_size = 0;

    return IFI_OK;
}
