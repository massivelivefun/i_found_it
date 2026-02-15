#ifndef FILE_CREATION_H
#define FILE_CREATION_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "wad3/miptex.h"
#include "wad3/wad3directoryentry.h"

#define MIPMAP_COUNT 4

extern const char * mipmap_suffixes[MIPMAP_COUNT];

int create_multi_alloc_output_file_paths(
    char ** paths,
    const char * output_path,
    const char * texture_name,
    const char ** texture_name_suffixes,
    const char * file_extension,
    size_t texture_count
);

int create_single_alloc_output_file_path(
    char ** path,
    const char * output_path,
    const char * texture_name,
    const char * texture_name_suffix,
    const char * file_extension
);

int create_picture(
    FILE * f,
    const char * input_file_path,
    const char * output_path,
    WAD3DirectoryEntry * directory_entries,
    uint32_t number_of_dirs
);

int create_textures_from_miptex(
    Arena * arena,
    const uint8_t * file_data,
    const char * output_path,
    uint32_t entry_offset,
    bool classic
);

int create_mipmap_zero(
    const char * path,
    const char * output_path,
    uint32_t width,
    uint32_t height,
    uint8_t * indices,
    uint8_t * rgb_data
);

int create_mipmap_one(
    const char * path,
    const char * output_path,
    const WAD3MipTexPaletteColorData * c,
    const WAD3MipTexBuffers * b,
    const WAD3MipTex * m
);

int create_mipmap_two(
    const char * path,
    const char * output_path,
    const WAD3MipTexPaletteColorData * c,
    const WAD3MipTexBuffers * b,
    const WAD3MipTex * m
);

int create_mipmap_three(
    const char * path,
    const char * output_path,
    const WAD3MipTexPaletteColorData * c,
    const WAD3MipTexBuffers * b,
    const WAD3MipTex * m
);

int classic_func(
    char ** paths,
    const char * output_path,
    const WAD3MipTexPaletteColorData * c,
    const WAD3MipTexBuffers * b,
    const WAD3MipTex * m
);

int modern_func(
    char ** paths,
    const char * output_path,
    const WAD3MipTexPaletteColorData * c,
    const WAD3MipTexBuffers * b,
    const WAD3MipTex * m
);

// no zero

int create_mipmap_one_modern(
    const char * path,
    const char * output_path,
    const WAD3MipTexPaletteColorData * c,
    const WAD3MipTexBuffers * b,
    const WAD3MipTex * m,
    uint8_t * rgb_data
);

int create_mipmap_two_modern(
    const char * path,
    const char * output_path,
    // const WAD3MipTexPaletteColorData * c,
    // const WAD3MipTexBuffers * b,
    const WAD3MipTex * m,
    uint8_t * rgb_data_one,
    uint8_t * rgb_data_two
);

int create_mipmap_three_modern(
    const char * path,
    const char * output_path,
    // const WAD3MipTexPaletteColorData * c,
    // const WAD3MipTexBuffers * b,
    const WAD3MipTex * m,
    uint8_t * rgb_data_two,
    uint8_t * rgb_data_three
);

#endif
