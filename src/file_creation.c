#include "file_creation.h"

#include "wad3/wad3directoryentry.h"
#include "wad3/wad3header.h"
#include "wad3/wad3pic.h"
#include "wad3/wad3font.h"

#include "ifi_errors.h"
#include "terminal.h"

const char * mipmap_suffixes[] = { "_mm0", "_mm1", "_mm2", "_mm3" };

// CONTRACT: Valid arguments need to be null-terminated strings.
// @param char * texture_name_suffixes - CANNOT be passed in with NULL
// 
int create_multi_arena_output_file_paths(
    Arena * arena,
    char ** paths,
    const char * output_path,
    const char * texture_name,
    const char ** texture_name_suffixes,
    const char * file_extension,
    size_t texture_count
) {
    if (arena == NULL || paths == NULL || output_path == NULL ||
            texture_name == NULL || (texture_name_suffixes == NULL &&
                texture_count > 0) || file_extension == NULL) {
        fprintf(stderr, "Arguments cannot be NULL.\n");
        return IFI_ERROR_NULL_ARGS;
    }

    for (size_t i = 0; i < texture_count; i++) {
        int result = create_single_arena_output_file_path(
            arena, &paths[i], output_path, texture_name,
            texture_name_suffixes[i], file_extension
        );

        if (result != IFI_OK) {
            fprintf(stderr, "Error creating path for MipMap_%zu.\n", i);
            return 1;
        }
    }
    return 0;
}

// CONTRACT: Valid arguments need to be null-terminated strings.
// EXCEPTION: @param char * texture_name_suffix - Can be passed in with NULL
int create_single_arena_output_file_path(
    Arena * arena,
    char ** path,
    const char * output_path,
    const char * texture_name,
    const char * texture_name_suffix,
    const char * file_extension
) {
    if (arena == NULL || path == NULL || output_path == NULL ||
        texture_name == NULL || file_extension == NULL) {
        fprintf(stderr, "Arguments cannot be NULL.\n");
        return IFI_ERROR_NULL_ARGS;
    }
    // The suffix can be NULL so then it will have an empty length
    size_t suffix_len = (texture_name_suffix == NULL) ? 0 :
        strlen(texture_name_suffix);
    // Include null terminator into the length with the plus one
    size_t len = strlen(output_path) + strlen(texture_name) + suffix_len +
        strlen(file_extension) + 1;
    char * output_file_name = (char *)arena_push(arena, len);
    if (output_file_name == NULL) {
        fprintf(stderr, "Failed to allocate memory for output file path.\n");
        return IFI_ERROR_MEMORY;
    }
    snprintf(
        output_file_name, len, "%s%s%s%s", output_path, texture_name,
        (texture_name_suffix == NULL) ? "" : texture_name_suffix,
        file_extension);
    *path = output_file_name;
    return 0;
}

int create_picture(
    Arena * arena,
    const uint8_t * file_data,
    const char * output_path,
    uint32_t entry_offset,
    char * path
) {
    const uint8_t * picture_data = file_data + entry_offset;

    WAD3Pic pic;
    init_wad3pic_from_data(&pic, picture_data);

    int picture_result = create_mipmap(
        path, pic.width, pic.height, pic.indices, pic.rgb_data
    );
    if (picture_result != 0) {
        fprintf(stderr, "Failed to create picture at: %s\n",
            output_path);
        return 1;
    }
    return 0;
}

int create_textures_from_miptex(
    Arena * arena,
    const uint8_t * file_data,
    const char * output_path,
    uint32_t entry_offset,
    char ** paths,
    bool classic
) {
    int status = IFI_OK;
    const uint8_t * miptex_data = file_data + entry_offset;

    WAD3MipTex m;
    init_wad3miptex_from_data(&m, miptex_data);

    WAD3MipTexBuffers b;
    init_wad3miptexbuffers_from_data(&b, &m, miptex_data);

    if (classic) {
        if (classic_func(paths, output_path, &b) != IFI_OK) {
            fprintf(stderr, "Classic failed.\n");
            status = IFI_ERROR_INVALID;
        }
    } else {
        if (modern_func(paths, output_path, &b) != IFI_OK) {
            fprintf(stderr, "Modern failed.\n");
            status = IFI_ERROR_INVALID;
        }
    }

    return status;
}

int create_font_sheet(
    Arena * arena,
    const uint8_t * file_data,
    const char * output_path,
    uint32_t entry_offset,
    const char * path
) {
    const uint8_t * font_data = file_data + entry_offset;
    
    WAD3Font font;
    init_wad3font_from_data(&font, font_data);

    if (create_mipmap(
        path, font.width, font.height,
        font.indices, font.rgb_data) != 0
    ) {
        fprintf(stderr, "Failed to create font image: %s\n", output_path);
        return 1;
    }

    return 0;
}

int modern_func(
    char ** paths,
    const char * output_path,
    const WAD3MipTexBuffers * b
) {
    uint8_t rgb_one[(b->width / 2) * (b->height / 2) * 3];
    uint8_t rgb_two[(b->width / 4) * (b->height / 4) * 3];
    uint8_t rgb_three[(b->width / 8) * (b->height / 8) * 3];

    // MipMap Zero
    if (create_mipmap(
        paths[0], b->width, b->height, b->mipmap_zero, b->rgb_data) != 0
    ) {
        fprintf(stderr, "Failed to create mipmap zero at: %s\n",
            output_path);
        return 1;
    }

    // MipMap One
    if (create_mipmap_modern(
        paths[1], b->width / 2, b->height / 2, b->mipmap_zero, b->rgb_data, NULL, rgb_one
    ) != 0) {
        fprintf(stderr, "Failed to create mipmap one at path: %s\n",
            output_path);
        return 1;
    }

    // MipMap Two
    if (create_mipmap_modern(
        paths[2], b->width / 4, b->height / 4, NULL, NULL, rgb_one, rgb_two
    ) != 0) {
        fprintf(stderr, "Failed to create mipmap two at path: %s\n",
            output_path);
        return 1;
    }

    // MipMap Two
    if (create_mipmap_modern(
        paths[3], b->width / 8, b->height / 8, NULL, NULL, rgb_two, rgb_three
    ) != 0) {
        fprintf(stderr, "Failed to create mipmap three at path: %s\n",
            output_path);
        return 1;
    }
    return 0;
}

int classic_func(
    char ** paths,
    const char * output_path,
    const WAD3MipTexBuffers * b
) {
    // MipMap Zero
    if (create_mipmap(
        paths[0], b->width, b->height,
        b->mipmap_zero, b->rgb_data) != 0
    ) {
        fprintf(stderr, "Failed to create mipmap zero at: %s\n",
            output_path);
        return 1;
    }

    // MipMap One
    if (create_mipmap(
        paths[1], b->width / 2, b->height / 2,
        b->mipmap_one, b->rgb_data) != 0
    ) {
        fprintf(stderr, "Failed to create mipmap one at: %s\n",
            output_path);
        return 1;
    }

    // MipMap Two
    if (create_mipmap(
        paths[2], b->width / 4, b->height / 4,
        b->mipmap_two, b->rgb_data) != 0
    ) {
        fprintf(stderr, "Failed to create mipmap two at: %s\n",
            output_path);
        return 1;
    }

    // MipMap Three
    if (create_mipmap(
        paths[3], b->width / 8, b->height / 8,
        b->mipmap_three, b->rgb_data) != 0
    ) {
        fprintf(stderr, "Failed to create mipmap three at: %s\n",
            output_path);
        return 1;
    }
    return 0;
}

int create_mipmap(
    const char * path,
    uint32_t width,
    uint32_t height,
    uint8_t * indices,
    uint8_t * rgb_data
) {
    FILE * f = fopen(path, "wb");
    if (f == NULL) { return 1; }

    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", width, height);
    fprintf(f, "255\n");

    size_t total_pixels = width * height;
    for (size_t i = 0; i < total_pixels; i += 1) {
        fputc(rgb_data[indices[i] * 3 + 0], f);
        fputc(rgb_data[indices[i] * 3 + 1], f);
        fputc(rgb_data[indices[i] * 3 + 2], f);
    }

    fclose(f);
    return 0;
}

// I don't like this but i did this to combine functions
// to be less crazy
int create_mipmap_modern(
    const char * path, 
    uint32_t dest_w,
    uint32_t dest_h, 
    const uint8_t * src_indices,
    const uint8_t * palette,
    const uint8_t * src_rgb,
    uint8_t * dest_rgb
) {
    FILE * f = fopen(path, "wb");
    if (f == NULL) { return 1; }

    fprintf(f, "P6\n%u %u\n255\n", dest_w, dest_h);
    uint32_t src_w = dest_w * 2;

    for (uint32_t y = 0; y < dest_h; y++) {
        for (uint32_t x = 0; x < dest_w; x++) {
            uint32_t src_x = x * 2;
            uint32_t src_y = y * 2;

            uint32_t r = 0, g = 0, b = 0;

            if (src_rgb != NULL) {
                uint32_t row_stride = src_w * 3;
                
                uint32_t p1 = (src_y * row_stride) + (src_x * 3);
                uint32_t p2 = (src_y * row_stride) + ((src_x + 1) * 3);
                uint32_t p3 = ((src_y + 1) * row_stride) + (src_x * 3);
                uint32_t p4 = ((src_y + 1) * row_stride) + ((src_x + 1) * 3);

                r = src_rgb[p1 + 0] + src_rgb[p2 + 0] +
                    src_rgb[p3 + 0] + src_rgb[p4 + 0];
                g = src_rgb[p1 + 1] + src_rgb[p2 + 1] +
                    src_rgb[p3 + 1] + src_rgb[p4 + 1];
                b = src_rgb[p1 + 2] + src_rgb[p2 + 2] +
                    src_rgb[p3 + 2] + src_rgb[p4 + 2];

            } else {
                uint32_t p1 = (src_y * src_w) + src_x;
                uint32_t p2 = (src_y * src_w) + (src_x + 1);
                uint32_t p3 = ((src_y + 1) * src_w) + src_x;
                uint32_t p4 = ((src_y + 1) * src_w) + (src_x + 1);

                r = palette[src_indices[p1] * 3 + 0] +
                    palette[src_indices[p2] * 3 + 0] + 
                    palette[src_indices[p3] * 3 + 0] +
                    palette[src_indices[p4] * 3 + 0];
                g = palette[src_indices[p1] * 3 + 1] +
                    palette[src_indices[p2] * 3 + 1] + 
                    palette[src_indices[p3] * 3 + 1] +
                    palette[src_indices[p4] * 3 + 1];
                b = palette[src_indices[p1] * 3 + 2] +
                    palette[src_indices[p2] * 3 + 2] + 
                    palette[src_indices[p3] * 3 + 2] +
                    palette[src_indices[p4] * 3 + 2];
            }

            fputc((uint8_t)(r / 4), f); 
            fputc((uint8_t)(g / 4), f); 
            fputc((uint8_t)(b / 4), f);

            uint32_t dest_idx = ((y * dest_w) + x) * 3;
            dest_rgb[dest_idx + 0] = (uint8_t)(r / 4);
            dest_rgb[dest_idx + 1] = (uint8_t)(g / 4);
            dest_rgb[dest_idx + 2] = (uint8_t)(b / 4);
        }
    }
    fclose(f);
    return 0;
}
