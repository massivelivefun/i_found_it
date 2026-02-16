#ifndef FILE_CREATION_H
#define FILE_CREATION_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "wad3/miptex.h"
#include "wad3/wad3directoryentry.h"
#include "wad3/wad3font.h"

#include "export_context.h"

#define MIPMAP_COUNT 4

extern const char * mipmap_suffixes[MIPMAP_COUNT];

int create_multi_arena_output_file_paths(
    ExportContext * ctx,
    char ** paths,
    const char ** texture_name_suffixes,
    const char * file_extension,
    size_t texture_count
);

int create_single_arena_output_file_path(
    ExportContext * ctx,
    char ** path,
    const char * texture_name_suffix,
    const char * file_extension
);

int create_picture(ExportContext * ctx, char * path);
int create_textures_from_miptex(ExportContext * ctx, char ** paths);

int create_font_sheet(
    ExportContext * ctx,
    const char * path,
    const char * json_path
);

int export_font_metrics_json(
    ExportContext * ctx,
    const WAD3Font * font,
    const char * json_path
);

int classic_func(
    const WAD3MipTexBuffers * b,
    const char * output_path,
    char ** paths
);

int modern_func(
    const WAD3MipTexBuffers * b,
    const char * output_path,
    char ** paths
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
