#include <stdio.h>

#include "wad3miptex.h"

int new_wad3miptex(FILE * f, WAD3MipTex * m) {
    size_t name_size = 16;
    // ignore the null terminator
    if (fread(&(m->name), sizeof(char), name_size - 1, f) < name_size - 1) {
        fprintf(stderr, "Failed to read texture_name\n");
        return 1;
    }
    fseek(f, sizeof(char), SEEK_CUR);
    m->name[15] = '\0';
    if (fread(&(m->width), sizeof(uint32_t), 1, f) < 1) {
        fprintf(stderr, "Failed to read width\n");
        return 1;
    }
    if (fread(&(m->height), sizeof(uint32_t), 1, f) < 1) {
        fprintf(stderr, "Failed to read height\n");
        return 1;
    }
    size_t offsets_size = 4;
    if (fread(m->offsets, sizeof(uint32_t), offsets_size, f) < offsets_size) {
        fprintf(stderr, "Failed to read offsets\n");
        return 1;
    }
    return 0;
}

void print_wad3miptex(WAD3MipTex * m) {
    printf("\ntexture_name: %s\n", m->name);
    printf("width: %d\n", m->width);
    printf("height: %d\n", m->height);
    for (size_t i = 0; i < 4; i += 1) {
        printf("offset[%zu]: %d\n", i, m->offsets[i]);
    }
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

int new_alloc_wad3miptexbuffers(
    FILE * f,
    WAD3MipTex * m,
    WAD3MipTexBuffers * b,
    int32_t entry_offset
) {
    if (f == NULL || m == NULL || b == NULL) {
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
        // No need to free anything, all of the buffers point to null
        return 1;
    }
    b->mipmap_zero_size = size_zero;

    b->mipmap_one = (uint8_t *)malloc(sizeof(uint8_t) * size_one);
    if (b->mipmap_one == NULL) {
        fprintf(stderr, "Failed to allocate mipmap_one.\n");
        free_alloc_wad3miptexbuffers(b);
        return 1;
    }
    b->mipmap_one_size = size_one;

    b->mipmap_two = (uint8_t *)malloc(sizeof(uint8_t) * size_two);
    if (b->mipmap_two == NULL) {
        fprintf(stderr, "Failed to allocate mipmap_two.\n");
        free_alloc_wad3miptexbuffers(b);
        return 1;
    }
    b->mipmap_two_size = size_two;

    b->mipmap_three = (uint8_t *)malloc(sizeof(uint8_t) * size_three);
    if (b->mipmap_three == NULL) {
        fprintf(stderr, "Failed to allocate mipmap_three.\n");
        free_alloc_wad3miptexbuffers(b);
        return 1;
    }
    b->mipmap_three_size = size_three;

    if (new_wad3miptexbuffers(f, m, b, entry_offset) != 0) {
        fprintf(stderr, "Failed to fill buffers with data.\n");
        free_alloc_wad3miptexbuffers(b);
        return 1;
    }

    return 0;
}

int free_alloc_wad3miptexbuffers(WAD3MipTexBuffers * b) {
    if (b == NULL) {
        return 0;
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

    return 0;
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


int new_alloc_WAD3MipTexPaletteColorData_from_file(
    WAD3MipTexPaletteColorData * c,
    FILE * f
) {
    if (c == NULL || f == NULL) {
        fprintf(stderr, "Arguments need to not be NULL.\n");
        return 1;
    }
    c->palette_size = 0;
    c->rgb_data = NULL;
    if (fread(&(c->palette_size), sizeof(uint16_t), 1, f) < 1) {
        fprintf(stderr, "Failed to read palette_size\n");
        return 1;
    }
    if (c->palette_size == 0) {
        fprintf(stderr, "Palette size in file is invalid.\n");
        return 1;
    }
    size_t rgb_data_size = c->palette_size * 3;
    c->rgb_data = (uint8_t *)malloc(sizeof(uint8_t) * rgb_data_size);
    if (c->rgb_data == NULL) {
        fprintf(stderr, "Failed to allocate rgb data.\n");
        free_WAD3MipTexPaletteColorData(c);
        return 1;
    }
    if (fread(c->rgb_data, sizeof(uint8_t), rgb_data_size, f) < rgb_data_size) {
        fprintf(stderr, "Failed to read data for rgb_data.\n");
        free_WAD3MipTexPaletteColorData(c);
        return 1;
    }
    return 0;
}

int free_WAD3MipTexPaletteColorData(WAD3MipTexPaletteColorData * c) {
    if (c == NULL) {
        return 0;
    }

    free(c->rgb_data);
    c->palette_size = 0;
    c->rgb_data = NULL;

    return 0;
}

// doesn't work
int new_wad3miptexpalettecolordata(FILE * f, WAD3MipTexPaletteColorData * c) {
    if (fread(&(c->palette_size), sizeof(uint16_t), 1, f) < 1) {
        fprintf(stderr, "Failed to read palette_size\n");
        return 1;
    }
    size_t rgb_data_size = c->palette_size * 3;
    if (fread(c->rgb_data, sizeof(uint8_t), rgb_data_size, f) < rgb_data_size) {
        fprintf(stderr, "Failed to read rgb_data\n");
        return 1;
    }
    return 0;
}

int set_wad3miptexpalettecolordata_palette_size(FILE * f, WAD3MipTexPaletteColorData * c) {
    if (fread(&(c->palette_size), sizeof(uint16_t), 1, f) < 1) {
        fprintf(stderr, "Failed to read palette_size\n");
        return 1;
    }
    return 0;
}

int set_wad3miptexpalettecolordata_rgb_data(FILE * f, WAD3MipTexPaletteColorData * c) {
    size_t rgb_data_size = c->palette_size * 3;
    if (fread(c->rgb_data, sizeof(uint8_t), rgb_data_size, f) < rgb_data_size) {
        fprintf(stderr, "Failed to read rgb_data\n");
        return 1;
    }
    return 0;
}

void print_wad3miptexpalettecolordata(WAD3MipTexPaletteColorData * c) {
    printf("palette_size: %d\n", c->palette_size);
    const size_t rgb_data_size = c->palette_size * 3;
    printf("rgb_data_size: %zu\n", rgb_data_size);
    for (size_t i = 0; i < rgb_data_size; i += 3) {
        printf("%d %d %d\n", c->rgb_data[i + 0], c->rgb_data[i + 1], c->rgb_data[i + 2]);
    }
}
