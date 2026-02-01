#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "wad3miptex.h"
#include "wad3directoryentry.h"
#include "wad3header.h"

void clear_terminal() {
#if _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int create_file_paths(char ** paths, const char * output_path, const char * texture_name) {
    size_t path_length = strlen(output_path);
    size_t texture_length = strlen(texture_name);
    size_t output_file_size_without_mipmap_and_file_extension =
        path_length + texture_length;

    size_t mipmap_size = 4;
    char * mipmap_text[mipmap_size];
    mipmap_text[0] = "_mm0";
    mipmap_text[1] = "_mm1";
    mipmap_text[2] = "_mm2";
    mipmap_text[3] = "_mm3";

    size_t mipmap_lengths[mipmap_size];
    for (size_t i = 0; i < mipmap_size; i += 1) {
        mipmap_lengths[i] = strlen(mipmap_text[i]);
    }

    char * file_extension = ".ppm";
    size_t file_extension_length = strlen(file_extension);

    for (size_t i = 0; i < mipmap_size; i += 1) {
        size_t output_file_size = output_file_size_without_mipmap_and_file_extension +
            mipmap_lengths[i] + file_extension_length + 1;
        char * output_file_name = (char *)malloc(sizeof(char) * output_file_size);
        strcpy(output_file_name, output_path);
        strcat(output_file_name, texture_name);
        strcat(output_file_name, mipmap_text[i]);
        strcat(output_file_name, file_extension);
        paths[i] = output_file_name;
    }
    return 0;
} 

int create_texture(
    FILE * f,
    const char * input_file_path,
    const char * output_path,
    WAD3DirectoryEntry * directory_entries,
    uint32_t number_of_dirs
) {
    uint32_t number;

    // if (scanf("%d", &number) == 1) {
    //     fprintf(stderr, "not a valid number scanned\n");
    //     return 1;
    // }

    printf("Enter an index shown in option #2: ");
    scanf(" %d", &number);

    if (number < 1 || number > number_of_dirs + 1) {
        fprintf(stderr, "not a valid number\n");
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

    size_t mipmap_paths_count = 4;
    char * paths[mipmap_paths_count];
    if (create_file_paths(paths, output_path, m.name)) {
        fprintf(stderr, "failed to create paths\n");
        return 1;
    }
    for (size_t i = 0; i < mipmap_paths_count - 1; i += 1) {
        printf("%s\n", paths[i]);
    }
    printf("%s", paths[mipmap_paths_count - 1]);


    FILE * f_zero = fopen(paths[0], "wb");
    if (f_zero == NULL) {
        fprintf(stderr, "Cannot write to file: %s\n", output_path);
        fclose(f_zero);
        return 1;
    }

    fprintf(f_zero, "P6\n");
    fprintf(f_zero, "%d %d\n", m.width, m.height);
    fprintf(f_zero, "255\n");

    for (size_t y = 0; y < m.height; y += 1) {
        for (size_t x = 0; x < m.width; x += 1) {
            size_t idx = y * m.width + x;
            fputc(c.rgb_data[b.mipmap_zero[idx] * 3 + 0], f_zero);
            fputc(c.rgb_data[b.mipmap_zero[idx] * 3 + 1], f_zero);
            fputc(c.rgb_data[b.mipmap_zero[idx] * 3 + 2], f_zero);
        }
    }

    fclose(f_zero);

    // containers CONTAINERS LMAO
    uint8_t rgb_data_one[(m.width / 2) * (m.height / 2) * 3];
    uint8_t rgb_data_two[(m.width / 4) * (m.height / 4) * 3];
    uint8_t rgb_data_three[(m.width / 8) * (m.height / 8) * 3];

    FILE * f_one = fopen(paths[1], "wb");
    if (f_one == NULL) {
        fprintf(stderr, "Cannot write to file: %s\n", output_path);
        fclose(f_one);
        return 1;
    }

    fprintf(f_one, "P6\n");
    fprintf(f_one, "%d %d\n", m.width / 2, m.height / 2);
    fprintf(f_one, "255\n");

    for (size_t y = 0; y < m.height / 2; y += 1) {
        for (size_t x = 0; x < m.width / 2; x += 1) {
            int src_x = x * 2;
            int src_y = y * 2;
            int src_w = m.width;

            int idx1 = (src_y * src_w + src_x);
            int idx2 = (src_y * src_w + src_x + 1);
            int idx3 = ((src_y + 1) * src_w + src_x);
            int idx4 = ((src_y + 1) * src_w + src_x + 1);

            int sum_r = c.rgb_data[b.mipmap_zero[idx1] * 3 + 0] +
                c.rgb_data[b.mipmap_zero[idx2] * 3 + 0] +
                c.rgb_data[b.mipmap_zero[idx3] * 3 + 0] +
                c.rgb_data[b.mipmap_zero[idx4] * 3 + 0];

            int sum_g = c.rgb_data[b.mipmap_zero[idx1] * 3 + 1] +
                c.rgb_data[b.mipmap_zero[idx2] * 3 + 1] +
                c.rgb_data[b.mipmap_zero[idx3] * 3 + 1] +
                c.rgb_data[b.mipmap_zero[idx4] * 3 + 1];

            int sum_b = c.rgb_data[b.mipmap_zero[idx1] * 3 + 2] +
                c.rgb_data[b.mipmap_zero[idx2] * 3 + 2] +
                c.rgb_data[b.mipmap_zero[idx3] * 3 + 2] +
                c.rgb_data[b.mipmap_zero[idx4] * 3 + 2];

            int dest_idx = (y * (m.width / 2) + x) * 3;

            fputc((uint8_t)(sum_r / 4), f_one);
            fputc((uint8_t)(sum_g / 4), f_one);
            fputc((uint8_t)(sum_b / 4), f_one);

            rgb_data_one[dest_idx + 0] = (uint8_t)(sum_r / 4);
            rgb_data_one[dest_idx + 1] = (uint8_t)(sum_g / 4);
            rgb_data_one[dest_idx + 2] = (uint8_t)(sum_b / 4);

            // Nearest Neighbor
            // int src_idx = (y * 2) * m.width + (x * 2);
            // fputc(c.rgb_data[b.mipmap_zero[src_idx] * 3 + 0], f_one);
            // fputc(c.rgb_data[b.mipmap_zero[src_idx] * 3 + 1], f_one);
            // fputc(c.rgb_data[b.mipmap_zero[src_idx] * 3 + 2], f_one);

            // Raw
            // size_t idx = y * m.width / 2 + x;
            // fputc(c.rgb_data[b.mipmap_one[idx] * 3 + 0], f_one);
            // fputc(c.rgb_data[b.mipmap_one[idx] * 3 + 1], f_one);
            // fputc(c.rgb_data[b.mipmap_one[idx] * 3 + 2], f_one);
        }
    }

    fclose(f_one);

    FILE * f_two = fopen(paths[2], "wb");
    if (f_two == NULL) {
        fprintf(stderr, "Cannot write to file: %s\n", output_path);
        fclose(f_two);
        return 1;
    }

    fprintf(f_two, "P6\n");
    fprintf(f_two, "%d %d\n", m.width / 4, m.height / 4);
    fprintf(f_two, "255\n");

    for (size_t y = 0; y < m.height / 4; y += 1) {
        for (size_t x = 0; x < m.width / 4; x += 1) {
            int src_x = x * 2;
            int src_y = y * 2;
            int src_w = m.width / 2;

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

            int dest_idx = (y * (m.width / 4) + x) * 3;

            fputc((uint8_t)(sum_r / 4), f_two);
            fputc((uint8_t)(sum_g / 4), f_two);
            fputc((uint8_t)(sum_b / 4), f_two);

            rgb_data_two[dest_idx + 0] = (uint8_t)(sum_r / 4);
            rgb_data_two[dest_idx + 1] = (uint8_t)(sum_g / 4);
            rgb_data_two[dest_idx + 2] = (uint8_t)(sum_b / 4);

            // Raw
            // size_t idx = y * m.width / 4 + x;
            // fputc(c.rgb_data[b.mipmap_two[idx] * 3 + 0], f_two);
            // fputc(c.rgb_data[b.mipmap_two[idx] * 3 + 1], f_two);
            // fputc(c.rgb_data[b.mipmap_two[idx] * 3 + 2], f_two);
        }
    }

    fclose(f_two);

    FILE * f_three = fopen(paths[3], "wb");
    if (f_three == NULL) {
        fprintf(stderr, "Cannot write to file: %s\n", output_path);
        fclose(f_three);
        return 1;
    }

    fprintf(f_three, "P6\n");
    fprintf(f_three, "%d %d\n", m.width / 8, m.height / 8);
    fprintf(f_three, "255\n");

    for (size_t y = 0; y < m.height / 8; y += 1) {
        for (size_t x = 0; x < m.width / 8; x += 1) {
            int src_x = x * 2;
            int src_y = y * 2;
            int src_w = m.width / 4;

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

            int dest_idx = (y * (m.width / 8) + x) * 3;

            fputc((uint8_t)(sum_r / 4), f_three);
            fputc((uint8_t)(sum_g / 4), f_three);
            fputc((uint8_t)(sum_b / 4), f_three);

            rgb_data_three[dest_idx + 0] = (uint8_t)(sum_r / 4);
            rgb_data_three[dest_idx + 1] = (uint8_t)(sum_g / 4);
            rgb_data_three[dest_idx + 2] = (uint8_t)(sum_b / 4);

            // Raw
            // size_t idx = y * m.width / 8 + x;
            // fputc(c.rgb_data[b.mipmap_three[idx] * 3 + 0], f_three);
            // fputc(c.rgb_data[b.mipmap_three[idx] * 3 + 1], f_three);
            // fputc(c.rgb_data[b.mipmap_three[idx] * 3 + 2], f_three);
        }
    }

    fclose(f_three);

    for (size_t i = 0; i < mipmap_paths_count; i += 1) {
        free(paths[i]);
    }
    return 0;
}

int main(int argc, char ** argv) {
    if (argc != 3) {
        fprintf(stderr, "Three args are needed: ./ifi <path/input_file.wad> "
            "<output_path>\n");
        return 1;
    }

    FILE * f = fopen(argv[1], "rb");
    if (f == NULL) {
        fprintf(stderr, "Cannot read file, doesn't exist: %s\n", argv[1]);
        return 1;
    }

    WAD3Header h;
    if (new_wad3header(f, &h) != 0) {
        fprintf(stderr, "Failed to read header: %s\n", argv[1]);
        fclose(f);
        return 1;
    }
    // print_wad3header(&h);

    // navigate file to start of directory entry from the start of the file
    fseek(f, h.dir_offset, SEEK_SET);

    WAD3DirectoryEntry directory_entries[h.num_dirs];
    for (size_t i = 0; i < h.num_dirs; i += 1) {
        WAD3DirectoryEntry d;
        if (new_wad3directoryentry(f, &d) != 0) {
            fprintf(stderr, "Failed to read directory entry: %s\n", argv[1]);
            fclose(f);
            return 1;
        }
        directory_entries[i] = d;
    }

    char choice;

    do {
        printf("\n========================\n");
        printf("       I FOUND IT        \n");
        printf("========================\n");
        printf(" 1. Create a texture <hardcoded atm>\n");
        printf(" 2. Print all directory entries in wad file\n");
        printf(" q. Quit\n");
        printf("------------------------\n");
        printf("Enter selection: ");

        scanf(" %c", &choice);

        switch (choice) {
            case '1': {
                    int result = create_texture(
                        f, argv[1], argv[2], directory_entries, h.num_dirs);
                    if (result != 0) {
                        fprintf(stderr, "Error creating the texture\n");
                    }
                    printf("\n");
                    // clear_terminal();
                }
                break;
            case '2':
                for (size_t i = 0; i < h.num_dirs; i += 1) {
                    printf("\n%zu", i + 1);
                    print_wad3directoryentry(&directory_entries[i]);
                }
                break;
            case 'q':
            case 'Q':
                printf("Quitting program... see you later\n");
                break;
            default:
                printf("\n(!) Invalid input. Please type 1-2 or q.\n");
                break;
        }
    } while (choice != 'q' && choice != 'Q');

    fclose(f);

    return 0;
}
