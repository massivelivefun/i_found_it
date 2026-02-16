#ifndef FILE_CREATION_H
#define FILE_CREATION_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "wad3/miptex.h"
#include "wad3/wad3directoryentry.h"

#define MIPMAP_COUNT 4

extern const char * mipmap_suffixes[MIPMAP_COUNT];

int create_multi_arena_output_file_paths(
    Arena * arena,
    char ** paths,
    const char * output_path,
    const char * texture_name,
    const char ** texture_name_suffixes,
    const char * file_extension,
    size_t texture_count
);

int create_single_arena_output_file_path(
    Arena * arena,
    char ** path,
    const char * output_path,
    const char * texture_name,
    const char * texture_name_suffix,
    const char * file_extension
);

int create_picture(
    Arena * arena,
    const uint8_t * file_data,
    const char * output_path,
    uint32_t entry_offset,
    char * path
);

int create_textures_from_miptex(
    Arena * arena,
    const uint8_t * file_data,
    const char * output_path,
    uint32_t entry_offset,
    char ** paths,
    bool classic
);

int create_font_sheet(
    Arena * arena,
    const uint8_t * file_data,
    const char * output_path,
    uint32_t entry_offset,
    const char * path
);

int classic_func(
    char ** paths,
    const char * output_path,
    const WAD3MipTexBuffers * b
);

int modern_func(
    char ** paths,
    const char * output_path,
    const WAD3MipTexBuffers * b
);

int create_mipmap(
    const char * path,
    uint32_t width,
    uint32_t height,
    uint8_t * indices,
    uint8_t * rgb_data
);

int create_mipmap_modern(
    const char * path, 
    uint32_t dest_w,
    uint32_t dest_h, 
    const uint8_t * src_indices,
    const uint8_t * palette,
    const uint8_t * src_rgb,
    uint8_t * dest_rgb
);

#endif
