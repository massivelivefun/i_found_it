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
    if (new_wad3miptexbuf(f, b->mipmap_zero_size, b->mipmap_zero, m->offsets[0], entry_offset)) {
        fprintf(stderr, "Failed to read mipmap_zero_size\n");
        return 1;
    }
    if (new_wad3miptexbuf(f, b->mipmap_one_size, b->mipmap_one, m->offsets[1], entry_offset)) {
        fprintf(stderr, "Failed to read mipmap_one_size\n");
        return 1;
    }
    if (new_wad3miptexbuf(f, b->mipmap_two_size, b->mipmap_two, m->offsets[2], entry_offset)) {
        fprintf(stderr, "Failed to read mipmap_two_size\n");
        return 1;
    }
    if (new_wad3miptexbuf(f, b->mipmap_three_size, b->mipmap_three, m->offsets[3], entry_offset)) {
        fprintf(stderr, "Failed to read mipmap_three_size\n");
        return 1;
    }
    return 0;
}

int new_wad3miptexbuf(FILE * f, size_t mipmap_size, uint8_t * mipmap_ptr, uint32_t offset, int32_t entry_offset) {
    fseek(f, entry_offset + offset, SEEK_SET);
    if (fread(mipmap_ptr, sizeof(uint8_t), mipmap_size, f) < mipmap_size) {
        fprintf(stderr, "Failed to read from file to put into mipmap\n");
        return 1;
    }
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
