#ifndef FILE_CREATION_H
#define FILE_CREATION_H

// #include <errno.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <stdbool.h>
// #include <string.h>

#include "wad3miptex.h"
#include "wad3directoryentry.h"
#include "wad3header.h"
#include "wad3pic.h"

#include "terminal.h"

int create_file_paths(
    char ** paths,
    const char * output_path,
    const char * texture_name
);

int create_single_allocated_output_file_path(
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

int create_texture(
    FILE * f,
    const char * input_file_path,
    const char * output_path,
    WAD3DirectoryEntry * directory_entries,
    uint32_t number_of_dirs,
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
