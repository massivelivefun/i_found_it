#include "file_creation.h"

// CONTRACT: Valid arguments need to be null-terminated strings.
// @param char * texture_name_suffixes - CANNOT be passed in with NULL
// 
int create_multi_alloc_output_file_paths(
    char ** paths,
    const char * output_path,
    const char * texture_name,
    const char ** texture_name_suffixes,
    const char * file_extension,
    size_t texture_count
) {
    if (paths == NULL || output_path == NULL || texture_name == NULL ||
        (texture_name_suffixes == NULL && texture_count > 0) ||
        file_extension == NULL) {
        fprintf(stderr, "Arguments to `create_multi_alloc_output_file_paths`"
            " cannot be null. Returning to caller.\n");
        return 1;
    }

    for (size_t i = 0; i < texture_count; i++) {
        int result = create_single_alloc_output_file_path(
            &paths[i], 
            output_path, 
            texture_name, 
            texture_name_suffixes[i], 
            file_extension
        );

        if (result != 0) {
            fprintf(stderr, "Error creating path for mipmap %zu.\n", i);
            // Free all of the valid allocations made, which is i
            for (size_t j = 0; j < i; j++) {
                free(paths[j]);
                paths[j] = NULL;
            }
            return 1;
        }
    }
    return 0;
}

// CONTRACT: Valid arguments need to be null-terminated strings.
// EXCEPTION: @param char * texture_name_suffix - Can be passed in with NULL
int create_single_alloc_output_file_path(
    char ** path,
    const char * output_path,
    const char * texture_name,
    const char * texture_name_suffix,
    const char * file_extension
) {
    if (path == NULL || output_path == NULL || texture_name == NULL ||
        file_extension == NULL) {
        fprintf(stderr, "Arguments to `create_single_allocated_output_file_"
            "path` cannot be null. Returning to caller.\n");
        return 1;
    }
    // The suffix can be NULL so then it will have an empty length
    size_t texture_name_suffix_len = (texture_name_suffix == NULL) ? 0 :
        strlen(texture_name_suffix);
    // Include null terminator into the length with the plus one
    size_t output_file_name_len = strlen(output_path) + strlen(texture_name) +
        texture_name_suffix_len + strlen(file_extension) + 1;
    char * output_file_name =
        (char *)malloc(sizeof(char) * output_file_name_len);
    if (output_file_name == NULL) {
        fprintf(stderr, "Failed to allocate memory for output file path."
            "Returning to caller.\n");
        return 1;
    }
    snprintf(output_file_name, output_file_name_len, "%s%s%s%s",
             output_path, texture_name,
             (texture_name_suffix == NULL) ? "" : texture_name_suffix,
             file_extension);
    *path = output_file_name;
    return 0;
}

int create_picture(
    FILE * f,
    const char * input_file_path,
    const char * output_path,
    WAD3DirectoryEntry * directory_entries,
    uint32_t number_of_dirs
) {
    uint32_t number;
    int select_result = handle_file_entry_select(&number, number_of_dirs);
    if (select_result != 0) {
        fprintf(stderr, "Failed to parse selection.\n");
        return 1;
    }

    int32_t entry_offset = directory_entries[number - 1].entry_offset;

    // seek to a specific file using magic numbers from the directory_entry
    // values
    // size_t slider = 12 + (87852 * 7) + (49772 * 2);
    // size_t slider = 2530548;
    fseek(f, entry_offset, SEEK_SET);

    WAD3Pic pic;
    int result = new_wad3pic(f, &pic);
    if (result != 0) {
        fprintf(stderr, "failed to create wad3pic: %s\n", input_file_path);
        return 1;
    }

    size_t mipmap_paths_count = 4;
    char * paths[mipmap_paths_count];
    if (create_single_alloc_output_file_path(
        paths, output_path, directory_entries[number - 1].texture_name,
        NULL, ".ppm"
    )) {
        fprintf(stderr, "failed to create paths\n");
        return 1;
    }
    // for (size_t i = 0; i < mipmap_paths_count - 1; i += 1) {
    //     printf("%s\n", paths[i]);
    // }
    // printf("%s", paths[mipmap_paths_count - 1]);

    int picture_result = create_mipmap_zero(
        paths[0], output_path, pic.width,
            pic.height,
            pic.indices,
            pic.rgb_data);
    if (picture_result != 0) {
        fprintf(stderr, "Failed to create mipmap zero\n");
        return 1;
    }

    free(pic.indices);
    free(pic.rgb_data);
    for (size_t i = 0; i < mipmap_paths_count; i += 1) {
        free(paths[i]);
    }
    return 0;
}

int create_texture(
    FILE * f,
    const char * input_file_path,
    const char * output_path,
    WAD3DirectoryEntry * directory_entries,
    uint32_t number_of_dirs,
    bool classic
) {
    uint32_t number;
    int result = handle_file_entry_select(&number, number_of_dirs);
    if (result != 0) {
        fprintf(stderr, "Failed to parse selection.\n");
        return 1;
    }

    int32_t entry_offset = directory_entries[number - 1].entry_offset;

    // seek to a specific file using magic numbers from the directory_entry
    // values
    // size_t slider = 12 + (87852 * 7) + (49772 * 2);
    // size_t slider = 2530548;
    fseek(f, entry_offset, SEEK_SET);

    WAD3MipTex m;
    if (new_wad3miptex(f, &m) != 0) {
        fprintf(stderr, "Failed to read mipmap texture: %s\n",
            input_file_path);
        fclose(f);
        return 1;
    }
    print_wad3miptex(&m);

    WAD3MipTexBuffers b;
    b.mipmap_zero_size = m.width * m.height;
    b.mipmap_one_size = (m.width / 2) * (m.height / 2);
    b.mipmap_two_size = (m.width / 4) * (m.height / 4);
    b.mipmap_three_size = (m.width / 8) * (m.height / 8);
    uint8_t mipmap_zero_indices[b.mipmap_zero_size];
    uint8_t mipmap_one_indices[b.mipmap_one_size];
    uint8_t mipmap_two_indices[b.mipmap_two_size];
    uint8_t mipmap_three_indices[b.mipmap_three_size];
    b.mipmap_zero = mipmap_zero_indices;
    b.mipmap_one = mipmap_one_indices;
    b.mipmap_two = mipmap_two_indices;
    b.mipmap_three = mipmap_three_indices;
    if (new_wad3miptexbuffers(f, &m, &b, entry_offset)) {
        fprintf(stderr, "Failed to read from file into mipmap buffers: %s\n",
            input_file_path);
        fclose(f);
        return 1;
    }

    int32_t palette_pos = entry_offset + m.offsets[3] + b.mipmap_three_size;
    fseek(f, palette_pos, SEEK_SET);

    WAD3MipTexPaletteColorData c;
    if (set_wad3miptexpalettecolordata_palette_size(f, &c)) {
        fprintf(stderr, "Failed to read palette_size\n");
        return 1;
    }
    uint8_t rgb_data[c.palette_size * 3];
    c.rgb_data = rgb_data;
    if (set_wad3miptexpalettecolordata_rgb_data(f, &c)) {
        fprintf(stderr, "Failed to read rgb_data\n");
        return 1;
    }
    // print_wad3miptexpalettecolordata(&c);

    const char * mipmap_suffixes[] = { "_mm0", "_mm1", "_mm2", "_mm3" };
    size_t mipmap_paths_count = 4;
    char * paths[mipmap_paths_count];
    if (create_multi_alloc_output_file_paths(
        paths, output_path, m.name,
        mipmap_suffixes, ".ppm", mipmap_paths_count
    )) {
        fprintf(stderr, "Failed to create paths.\n");
        return 1;
    }
    for (size_t i = 0; i < mipmap_paths_count - 1; i += 1) {
        printf("%s\n", paths[i]);
    }
    printf("%s", paths[mipmap_paths_count - 1]);

    if (classic) {
        int classic_result = classic_func(paths, output_path, &c, &b, &m);
        if (classic_result != 0) {
            fprintf(stderr, "classic failed.\n");
            for (size_t i = 0; i < mipmap_paths_count; i += 1) {
                free(paths[i]);
            }
            return 1;
        }
    } else {
        int modern_result = modern_func(paths, output_path, &c, &b, &m);
        if (modern_result != 0) {
            fprintf(stderr, "modern failed.\n");
            for (size_t i = 0; i < mipmap_paths_count; i += 1) {
                free(paths[i]);
            }
            return 1;
        }
    }

    for (size_t i = 0; i < mipmap_paths_count; i += 1) {
        free(paths[i]);
    }
    return 0;
}

int modern_func(
    char ** paths,
    const char * output_path,
    const WAD3MipTexPaletteColorData * c,
    const WAD3MipTexBuffers * b,
    const WAD3MipTex * m
) {
    uint8_t rgb_data_one[(m->width / 2) * (m->height / 2) * 3];
    uint8_t rgb_data_two[(m->width / 4) * (m->height / 4) * 3];
    uint8_t rgb_data_three[(m->width / 8) * (m->height / 8) * 3];

    int mm_zero_result = create_mipmap_zero(
        paths[0], output_path, m->width,
            m->height,
            b->mipmap_zero,
            c->rgb_data);
    if (mm_zero_result != 0) {
        fprintf(stderr, "Failed to create mipmap zero\n");
        return 1;
    }
    int mm_one_result = create_mipmap_one_modern(
        paths[1], output_path, c, b, m, rgb_data_one);
    if (mm_one_result != 0) {
        fprintf(stderr, "Failed to create mipmap one\n");
        return 1;
    }
    int mm_two_result = create_mipmap_two_modern(
        paths[2], output_path, m, rgb_data_one, rgb_data_two);
    if (mm_two_result != 0) {
        fprintf(stderr, "Failed to create mipmap two\n");
        return 1;
    }
    int mm_three_result = create_mipmap_three_modern(
        paths[3], output_path, m, rgb_data_two, rgb_data_three);
    if (mm_three_result != 0) {
        fprintf(stderr, "Failed to create mipmap three\n");
        return 1;
    }
    return 0;
}

int classic_func(
    char ** paths,
    const char * output_path,
    const WAD3MipTexPaletteColorData * c,
    const WAD3MipTexBuffers * b,
    const WAD3MipTex * m
) {
    int mm_zero_result = create_mipmap_zero(
        paths[0], output_path, m->width,
        m->height, b->mipmap_zero, c->rgb_data);
    if (mm_zero_result != 0) {
        fprintf(stderr, "Failed to create mipmap zero\n");
        return 1;
    }
    int mm_one_result = create_mipmap_one(paths[1], output_path, c, b, m);
    if (mm_one_result != 0) {
        fprintf(stderr, "Failed to create mipmap one\n");
        return 1;
    }
    int mm_two_result = create_mipmap_two(paths[2], output_path, c, b, m);
    if (mm_two_result != 0) {
        fprintf(stderr, "Failed to create mipmap two\n");
        return 1;
    }
    int mm_three_result = create_mipmap_three(paths[3], output_path, c, b, m);
    if (mm_three_result != 0) {
        fprintf(stderr, "Failed to create mipmap three\n");
        return 1;
    }
    return 0;
}

int create_mipmap_zero(
    const char * path,
    const char * output_path,
    uint32_t width,
    uint32_t height,
    uint8_t * indices,
    uint8_t * rgb_data
) {
    FILE * f = fopen(path, "wb");
    if (f == NULL) {
        fprintf(stderr, "Cannot write to file: %s\n", output_path);
        fclose(f);
        return 1;
    }

    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", width, height);
    fprintf(f, "255\n");

    for (size_t y = 0; y < height; y += 1) {
        for (size_t x = 0; x < width; x += 1) {
            size_t idx = y * width + x;
            fputc(rgb_data[indices[idx] * 3 + 0], f);
            fputc(rgb_data[indices[idx] * 3 + 1], f);
            fputc(rgb_data[indices[idx] * 3 + 2], f);
        }
    }

    fclose(f);
    return 0;
}

int create_mipmap_one(
    const char * path,
    const char * output_path,
    const WAD3MipTexPaletteColorData * c,
    const WAD3MipTexBuffers * b,
    const WAD3MipTex * m
) {
    FILE * f = fopen(path, "wb");
    if (f == NULL) {
        fprintf(stderr, "Cannot write to file: %s\n", output_path);
        fclose(f);
        return 1;
    }

    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", m->width / 2, m->height / 2);
    fprintf(f, "255\n");

    for (size_t y = 0; y < (m->height / 2); y += 1) {
        for (size_t x = 0; x < (m->width / 2); x += 1) {
            size_t idx = y * (m->width / 2) + x;
            fputc(c->rgb_data[b->mipmap_one[idx] * 3 + 0], f);
            fputc(c->rgb_data[b->mipmap_one[idx] * 3 + 1], f);
            fputc(c->rgb_data[b->mipmap_one[idx] * 3 + 2], f);
        }
    }

    fclose(f);
    return 0;
}

int create_mipmap_two(
    const char * path,
    const char * output_path,
    const WAD3MipTexPaletteColorData * c,
    const WAD3MipTexBuffers * b,
    const WAD3MipTex * m
) {
    FILE * f = fopen(path, "wb");
    if (f == NULL) {
        fprintf(stderr, "Cannot write to file: %s\n", output_path);
        fclose(f);
        return 1;
    }

    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", m->width / 4, m->height / 4);
    fprintf(f, "255\n");

    for (size_t y = 0; y < (m->height / 4); y += 1) {
        for (size_t x = 0; x < (m->width / 4); x += 1) {
            size_t idx = y * (m->width / 4) + x;
            fputc(c->rgb_data[b->mipmap_two[idx] * 3 + 0], f);
            fputc(c->rgb_data[b->mipmap_two[idx] * 3 + 1], f);
            fputc(c->rgb_data[b->mipmap_two[idx] * 3 + 2], f);
        }
    }

    fclose(f);
    return 0;
}

int create_mipmap_three(
    const char * path,
    const char * output_path,
    const WAD3MipTexPaletteColorData * c,
    const WAD3MipTexBuffers * b,
    const WAD3MipTex * m
) {
    FILE * f = fopen(path, "wb");
    if (f == NULL) {
        fprintf(stderr, "Cannot write to file: %s\n", output_path);
        fclose(f);
        return 1;
    }

    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", m->width / 8, m->height / 8);
    fprintf(f, "255\n");

    for (size_t y = 0; y < (m->height / 8); y += 1) {
        for (size_t x = 0; x < (m->width / 8); x += 1) {
            size_t idx = y * (m->width / 8) + x;
            fputc(c->rgb_data[b->mipmap_three[idx] * 3 + 0], f);
            fputc(c->rgb_data[b->mipmap_three[idx] * 3 + 1], f);
            fputc(c->rgb_data[b->mipmap_three[idx] * 3 + 2], f);
        }
    }

    fclose(f);
    return 0;
}

int create_mipmap_one_modern(
    const char * path,
    const char * output_path,
    const WAD3MipTexPaletteColorData * c,
    const WAD3MipTexBuffers * b,
    const WAD3MipTex * m,
    uint8_t * rgb_data
) {
    FILE * f = fopen(path, "wb");
    if (f == NULL) {
        fprintf(stderr, "Cannot write to file: %s\n", output_path);
        fclose(f);
        return 1;
    }

    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", m->width / 2, m->height / 2);
    fprintf(f, "255\n");

    for (size_t y = 0; y < m->height / 2; y += 1) {
        for (size_t x = 0; x < m->width / 2; x += 1) {
            int src_x = x * 2;
            int src_y = y * 2;
            int src_w = m->width;

            int idx1 = (src_y * src_w + src_x);
            int idx2 = (src_y * src_w + src_x + 1);
            int idx3 = ((src_y + 1) * src_w + src_x);
            int idx4 = ((src_y + 1) * src_w + src_x + 1);

            int sum_r = c->rgb_data[b->mipmap_zero[idx1] * 3 + 0] +
                c->rgb_data[b->mipmap_zero[idx2] * 3 + 0] +
                c->rgb_data[b->mipmap_zero[idx3] * 3 + 0] +
                c->rgb_data[b->mipmap_zero[idx4] * 3 + 0];

            int sum_g = c->rgb_data[b->mipmap_zero[idx1] * 3 + 1] +
                c->rgb_data[b->mipmap_zero[idx2] * 3 + 1] +
                c->rgb_data[b->mipmap_zero[idx3] * 3 + 1] +
                c->rgb_data[b->mipmap_zero[idx4] * 3 + 1];

            int sum_b = c->rgb_data[b->mipmap_zero[idx1] * 3 + 2] +
                c->rgb_data[b->mipmap_zero[idx2] * 3 + 2] +
                c->rgb_data[b->mipmap_zero[idx3] * 3 + 2] +
                c->rgb_data[b->mipmap_zero[idx4] * 3 + 2];

            int dest_idx = (y * (m->width / 2) + x) * 3;

            fputc((uint8_t)(sum_r / 4), f);
            fputc((uint8_t)(sum_g / 4), f);
            fputc((uint8_t)(sum_b / 4), f);

            rgb_data[dest_idx + 0] = (uint8_t)(sum_r / 4);
            rgb_data[dest_idx + 1] = (uint8_t)(sum_g / 4);
            rgb_data[dest_idx + 2] = (uint8_t)(sum_b / 4);
        }
    }

    fclose(f);
    return 0;
}

int create_mipmap_two_modern(
    const char * path,
    const char * output_path,
    const WAD3MipTex * m,
    uint8_t * rgb_data_one,
    uint8_t * rgb_data_two
) {
    FILE * f = fopen(path, "wb");
    if (f == NULL) {
        fprintf(stderr, "Cannot write to file: %s\n", output_path);
        fclose(f);
        return 1;
    }

    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", m->width / 4, m->height / 4);
    fprintf(f, "255\n");

    for (size_t y = 0; y < m->height / 4; y += 1) {
        for (size_t x = 0; x < m->width / 4; x += 1) {
            int src_x = x * 2;
            int src_y = y * 2;
            int src_w = m->width / 2;

            int idx1 = (src_y * src_w + src_x);
            int idx2 = (src_y * src_w + src_x + 1);
            int idx3 = ((src_y + 1) * src_w + src_x);
            int idx4 = ((src_y + 1) * src_w + src_x + 1);

            int sum_r = rgb_data_one[idx1 * 3 + 0] +
                rgb_data_one[idx2 * 3 + 0] +
                rgb_data_one[idx3 * 3 + 0] +
                rgb_data_one[idx4 * 3 + 0];

            int sum_g = rgb_data_one[idx1 * 3 + 1] +
                rgb_data_one[idx2 * 3 + 1] +
                rgb_data_one[idx3 * 3 + 1] +
                rgb_data_one[idx4 * 3 + 1];

            int sum_b = rgb_data_one[idx1 * 3 + 2] +
                rgb_data_one[idx2 * 3 + 2] +
                rgb_data_one[idx3 * 3 + 2] +
                rgb_data_one[idx4 * 3 + 2];

            int dest_idx = (y * (m->width / 4) + x) * 3;

            fputc((uint8_t)(sum_r / 4), f);
            fputc((uint8_t)(sum_g / 4), f);
            fputc((uint8_t)(sum_b / 4), f);

            rgb_data_two[dest_idx + 0] = (uint8_t)(sum_r / 4);
            rgb_data_two[dest_idx + 1] = (uint8_t)(sum_g / 4);
            rgb_data_two[dest_idx + 2] = (uint8_t)(sum_b / 4);
        }
    }

    fclose(f);
    return 0;
}

int create_mipmap_three_modern(
    const char * path,
    const char * output_path,
    const WAD3MipTex * m,
    uint8_t * rgb_data_two,
    uint8_t * rgb_data_three
) {
    FILE * f = fopen(path, "wb");
    if (f == NULL) {
        fprintf(stderr, "Cannot write to file: %s\n", output_path);
        fclose(f);
        return 1;
    }

    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", m->width / 8, m->height / 8);
    fprintf(f, "255\n");

    for (size_t y = 0; y < m->height / 8; y += 1) {
        for (size_t x = 0; x < m->width / 8; x += 1) {
            int src_x = x * 2;
            int src_y = y * 2;
            int src_w = m->width / 4;

            int idx1 = (src_y * src_w + src_x);
            int idx2 = (src_y * src_w + src_x + 1);
            int idx3 = ((src_y + 1) * src_w + src_x);
            int idx4 = ((src_y + 1) * src_w + src_x + 1);

            int sum_r = rgb_data_two[idx1 * 3 + 0] +
                rgb_data_two[idx2 * 3 + 0] +
                rgb_data_two[idx3 * 3 + 0] +
                rgb_data_two[idx4 * 3 + 0];

            int sum_g = rgb_data_two[idx1 * 3 + 1] +
                rgb_data_two[idx2 * 3 + 1] +
                rgb_data_two[idx3 * 3 + 1] +
                rgb_data_two[idx4 * 3 + 1];

            int sum_b = rgb_data_two[idx1 * 3 + 2] +
                rgb_data_two[idx2 * 3 + 2] +
                rgb_data_two[idx3 * 3 + 2] +
                rgb_data_two[idx4 * 3 + 2];

            int dest_idx = (y * (m->width / 8) + x) * 3;

            fputc((uint8_t)(sum_r / 4), f);
            fputc((uint8_t)(sum_g / 4), f);
            fputc((uint8_t)(sum_b / 4), f);

            rgb_data_three[dest_idx + 0] = (uint8_t)(sum_r / 4);
            rgb_data_three[dest_idx + 1] = (uint8_t)(sum_g / 4);
            rgb_data_three[dest_idx + 2] = (uint8_t)(sum_b / 4);
        }
    }

    fclose(f);
    return 0;
}
