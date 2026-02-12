#include "wad3miptex.h"

#include <stdio.h>

#include "endian_utils.h"
#include "file_utils.h"
#include "ifi_errors.h"

int new_wad3miptex_from_file(WAD3MipTex * m, FILE * f) {
    if (m == NULL || f == NULL) { return IFI_ERROR_NULL_ARGS; }
    if (read_fixed_string_from_file(f, m->name, MAX_TEXTURE_NAME)) {
        return IFI_ERROR_READ;
    }
    if (read_u32le_from_file(f, &m->width)) { return IFI_ERROR_READ; }
    if (read_u32le_from_file(f, &m->height)) { return IFI_ERROR_READ; }
    for (size_t i = 0; i < MAX_OFFSETS; i += 1) {
        if (read_u32le_from_file(f, &m->offsets[i])) {
            return IFI_ERROR_READ;
        }
    }
    return IFI_OK;
}

int print_wad3miptex(const WAD3MipTex * m) {
    if (m == NULL) { return IFI_ERROR_NULL_ARGS; }
    int len = snprintf(NULL, 0, "\nname: %.16s\nwidth: %d\nheight: %d\n",
        m->name, m->width, m->height);
    if (len < 0) { return IFI_ERROR_ENCODING; }
    size_t total_len = (size_t)len;
    for (size_t i = 0; i < MAX_OFFSETS; i += 1) {
        len = snprintf(NULL, 0, "offset[%zu]: %d\n", i, m->offsets[i]);
        if (len < 0) { return IFI_ERROR_ENCODING; }
        total_len += len;
    }
    char * buffer = (char *)malloc(total_len + 1);
    if (buffer == NULL) { return IFI_ERROR_MEMORY; }
    char * ptr = buffer;
    ptr += sprintf(ptr, "\nname: %.16s\nwidth: %d\nheight: %d\n",
        m->name, m->width, m->height);
    for (size_t i = 0; i < MAX_OFFSETS; i += 1) {
        ptr += sprintf(ptr, "offset[%zu]: %d\n", i, m->offsets[i]);
    }
    printf("%s", buffer);
    free(buffer);
    return IFI_OK;
}

int new_wad3miptexbuffers(FILE * f, WAD3MipTex * m, WAD3MipTexBuffers * b, int32_t entry_offset) {
    if (f == NULL || m == NULL || b == NULL) {
        return 1;
    }
    if (new_wad3miptexbuf(f, b->mipmap_zero_size, b->mipmap_zero, m->offsets[0], entry_offset)) {
        fprintf(stderr, "Failed to read mipmap_zero data.\n");
        return 1;
    }
    if (new_wad3miptexbuf(f, b->mipmap_one_size, b->mipmap_one, m->offsets[1], entry_offset)) {
        fprintf(stderr, "Failed to read mipmap_one data.\n");
        return 1;
    }
    if (new_wad3miptexbuf(f, b->mipmap_two_size, b->mipmap_two, m->offsets[2], entry_offset)) {
        fprintf(stderr, "Failed to read mipmap_two data.\n");
        return 1;
    }
    if (new_wad3miptexbuf(f, b->mipmap_three_size, b->mipmap_three, m->offsets[3], entry_offset)) {
        fprintf(stderr, "Failed to read mipmap_three data.\n");
        return 1;
    }
    return 0;
}

int new_alloc_wad3miptexbuffers_from_file(
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
    free_alloc_wad3miptexbuffers(b);
    return result;
}

int free_alloc_wad3miptexbuffers(WAD3MipTexBuffers * b) {
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

int new_wad3miptexbuf(FILE * f, size_t mipmap_size, uint8_t * mipmap_ptr, uint32_t offset, int32_t entry_offset) {
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


int new_alloc_wad3miptexpalettecolordata_from_file(
    WAD3MipTexPaletteColorData * c,
    FILE * f
) {
    if (c == NULL || f == NULL) {
        fprintf(stderr, "Arguments cannot be NULL.\n");
        return IFI_ERROR_NULL_ARGS;
    }
    int result = IFI_OK;

    c->palette_size = 0;
    c->rgb_data = NULL;

    if (read_u16le_from_file(f, &c->palette_size)) {
        fprintf(stderr, "Failed to read palette size.\n");
        result = IFI_ERROR_READ;
        goto error_cleanup;
    }
    if (c->palette_size == 0) {
        fprintf(stderr, "Palette size is invalid (0).\n");
        result = IFI_ERROR_INVALID;
        goto error_cleanup;
    }
    size_t rgb_data_size = c->palette_size * 3;
    c->rgb_data = (uint8_t *)malloc(sizeof(uint8_t) * rgb_data_size);
    if (c->rgb_data == NULL) {
        fprintf(stderr, "Failed to allocate rgb data.\n");
        result = IFI_ERROR_MEMORY;
        goto error_cleanup;
    }
    if (fread(c->rgb_data, sizeof(uint8_t), rgb_data_size, f) != rgb_data_size) {
        fprintf(stderr, "Failed to read palette data.\n");
        result = IFI_ERROR_READ;
        goto error_cleanup;
    }
    return result;
    
error_cleanup:
    free_wad3miptexpalettecolordata(c);
    return result;
}

int free_wad3miptexpalettecolordata(WAD3MipTexPaletteColorData * c) {
    if (c == NULL) {
        return IFI_ERROR_NULL_ARGS;
    }

    free(c->rgb_data);
    c->palette_size = 0;
    c->rgb_data = NULL;

    return IFI_OK;
}

// doesn't work
// int new_wad3miptexpalettecolordata(FILE * f, WAD3MipTexPaletteColorData * c) {
//     if (fread(&(c->palette_size), sizeof(uint16_t), 1, f) < 1) {
//         fprintf(stderr, "Failed to read palette_size\n");
//         return 1;
//     }
//     size_t rgb_data_size = c->palette_size * 3;
//     if (fread(c->rgb_data, sizeof(uint8_t), rgb_data_size, f) < rgb_data_size) {
//         fprintf(stderr, "Failed to read rgb_data\n");
//         return 1;
//     }
//     return 0;
// }

// int set_wad3miptexpalettecolordata_palette_size(FILE * f, WAD3MipTexPaletteColorData * c) {
//     if (fread(&(c->palette_size), sizeof(uint16_t), 1, f) < 1) {
//         fprintf(stderr, "Failed to read palette_size\n");
//         return 1;
//     }
//     return 0;
// }

// int set_wad3miptexpalettecolordata_rgb_data(FILE * f, WAD3MipTexPaletteColorData * c) {
//     size_t rgb_data_size = c->palette_size * 3;
//     if (fread(c->rgb_data, sizeof(uint8_t), rgb_data_size, f) < rgb_data_size) {
//         fprintf(stderr, "Failed to read rgb_data\n");
//         return 1;
//     }
//     return 0;
// }

int print_wad3miptexpalettecolordata(const WAD3MipTexPaletteColorData * c) {
    if (c == NULL) { return IFI_ERROR_NULL_ARGS; }
    int len = snprintf(NULL, 0, "\npalette_size: %d\nrgb_data_size: %d\n",
        c->palette_size, c->palette_size * 3);
    if (len < 0) { return IFI_ERROR_ENCODING; }
    size_t total_len = (size_t)len;
    for (size_t i = 0; i < c->palette_size * 3; i += 1) {
        len = snprintf(NULL, 0, "%d %d %d\n",
            c->rgb_data[i + 0], c->rgb_data[i + 1], c->rgb_data[i + 2]);
        if (len < 0) { return IFI_ERROR_ENCODING; }
        total_len += len;
    }
    char * buffer = (char *)malloc(total_len + 1);
    if (buffer == NULL) { return IFI_ERROR_MEMORY; }
    char * ptr = buffer;
    ptr += sprintf(ptr, "\npalette_size: %d\nrgb_data_size: %d\n",
        c->palette_size, c->palette_size * 3);
    for (size_t i = 0; i < c->palette_size * 3; i += 1) {
        ptr += sprintf(ptr, "%d %d %d\n",
            c->rgb_data[i + 0], c->rgb_data[i + 1], c->rgb_data[i + 2]);
    }
    printf("%s", buffer);
    free(buffer);
    return IFI_OK;
}
