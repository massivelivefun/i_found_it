#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "wad3/wad3directoryentry.h"
#include "wad3/wad3header.h"
#include "wad3/wad3pic.h"
#include "wad3/wad3font.h"
#include "wad3/wad3filetypes.h"

#include "utils.h"
#include "file_creation.h"
#include "file_mapper.h"
#include "terminal.h"

int file_type_operations(
    Arena * arena,
    const char file_type,
    const char * safe_texture_name,
    const char * output_path,
    uint32_t entry_offset,
    MappedFile * file,
    bool classic
) {
    switch (file_type) {
    case PIC: {
        // Picture, Qpic
        char * path;
        if (create_single_arena_output_file_path(
            arena, &path, output_path,
            safe_texture_name, "", ".ppm") != IFI_OK
        ) {
            fprintf(stderr, "Failed to create path.\n");
            return 1;
        }
        if (create_picture(
            arena, file->data, output_path,
            entry_offset, path) != IFI_OK
        ) {
            fprintf(stderr, "Error creating the picture.\n");
            return 1;
        }
        break;
    }
    case LUMPY:
    case MIPTEX: {
        // Mipmap Texture
        char * paths[MIPMAP_COUNT];
        if (create_multi_arena_output_file_paths(
            arena, paths, output_path, safe_texture_name,
            mipmap_suffixes, ".ppm", MIPMAP_COUNT) != IFI_OK
        ) {
            fprintf(stderr, "Failed to create paths.\n");
            return 1;
        }
        if (create_textures_from_miptex(
            arena, file->data, output_path,
            entry_offset, paths, classic) != IFI_OK
        ) {
            fprintf(stderr, "Error creating the textures.\n");
            return 1;
        }
        break;
    }
    case FONT: {
        // Font Sprite Sheet
        char * path;
        if (create_single_arena_output_file_path(
            arena, &path, output_path,
            safe_texture_name, "", ".ppm") != IFI_OK
        ) {
            fprintf(stderr, "Failed to create ppm file.\n");
            return 1;
        }
        char * json_path;
        if (create_single_arena_output_file_path(
            arena, &json_path, output_path,
            safe_texture_name, "", ".json") != IFI_OK
        ) {
            fprintf(stderr, "Failed to create json.\n");
            return 1;
        }
        if (create_font_sheet(
            arena, file->data, output_path,
            entry_offset, path, json_path,
            safe_texture_name) != IFI_OK
        ) {
            fprintf(stderr, "Error creating the font.\n");
            return 1;
        }
        break;
    }
    default:
        // Not a wad from goldsrc engine
        printf("Unsupported file_type.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char ** argv) {
    int result = EXIT_SUCCESS;
    WAD3DirectoryEntry * directory_entries = NULL;
    char * program_name = argv[0];

    if (argc != 3) {
        fprintf(stderr, "Three args are needed: %s <path/input_file.wad> "
            "<output_path>\n", program_name);
        result = EXIT_FAILURE;
        goto return_from_main;
    }

    char * input_file_path = argv[1];
    char * output_path = argv[2];

    MappedFile wad_file = {0};
    if (!map_file(&wad_file, input_file_path)) {
        result = EXIT_FAILURE;
        goto return_from_main;
    }

    // Pass the pure byte data directly to your validator
    if (!validate_wad_structure(wad_file.data, wad_file.size)) {
        fprintf(stderr, "Invalid WAD structure!\n");
        result = EXIT_FAILURE;
        goto unmap_file;
    }

    Arena wad_arena = {0};
    // Allocate 10 megabytes of information
    if (!arena_init(&wad_arena, 1024 * 1024 * 10)) {
        fprintf(stderr, "Fatal: Could not allocate memory arena.\n");
        result = EXIT_FAILURE;
        goto unmap_file;
    }

    WAD3Header h = {0};
    if (init_wad3header_from_data(&h, wad_file.data) != IFI_OK) {
        fprintf(stderr, "Failed to read wad3 header: %s\n",
            input_file_path);
        result = EXIT_FAILURE;
        goto cleanup;
    }
    if (validate_wad3header_magic(&h) != IFI_OK) {
        result = EXIT_FAILURE;
        goto cleanup;
    }

    directory_entries = parse_directories_from_data(
        &wad_arena, wad_file.data, h.dir_offset, h.num_dirs
    );
    if (directory_entries == NULL) {
        fprintf(stderr, "Fatal: Arena out of memory while parsing "
            "directories.\n");
        result = EXIT_FAILURE;
        goto cleanup;
    }

    bool classic = false;
    char menu_buffer[32];
    bool quit = false;

    do {
        print_menu();

        if (fgets(menu_buffer, sizeof(menu_buffer), stdin) == NULL) {
            break;
        }

        menu_buffer[strcspn(menu_buffer, "\n")] = 0;

        for (size_t i = 0; menu_buffer[i] != '\0'; i += 1) {
            menu_buffer[i] = (char)tolower((unsigned char)menu_buffer[i]);
        }

        if (strcmp(menu_buffer, "q") == 0 ||
            strcmp(menu_buffer, "quit") == 0) {
            printf("Quitting program.\n");
            quit = true;
            break;
        }

        char * endptr;
        int choice = (int)strtol(menu_buffer, &endptr, 10);

        if (endptr == menu_buffer || *endptr != '\0') {
            choice = -1;
        }

        switch (choice) {
            case 1:
                if (make_dir(output_path) == IFI_OK) {
                    printf("Directory created successfully: %s\n",
                        output_path);
                } else {
                    if (errno != EEXIST) {
                        perror("Error creating directory.\n");
                        break;
                    }
                }
                uint32_t number = 0;
                if (handle_file_entry_select(&number, h.num_dirs)
                    != IFI_OK) {
                    fprintf(stderr, "Failed to parse selection.\n");
                    break;
                }

                WAD3DirectoryEntry * entry = &(directory_entries[number - 1]);
                uint32_t entry_offset = entry->entry_offset;
                const char * texture_name = entry->texture_name;
                const char file_type = entry->file_type;

                // use safe texture name
                char safe_texture_name[16] = {0};
                memcpy(safe_texture_name, texture_name, 16);
                safe_texture_name[15] = '\0';

                size_t temp_memory_mark = arena_save(&wad_arena);

                if (file_type_operations(
                    &wad_arena, file_type, safe_texture_name,
                    output_path, entry_offset, &wad_file, classic)
                ) {
                    fprintf(stderr, "file_type_operation failed.\n");
                }

                arena_restore(&wad_arena, temp_memory_mark);
                break;
            case 2:
                for (size_t i = 0; i < h.num_dirs; i += 1) {
                    printf("\n%zu", i + 1);
                    print_wad3directoryentry(&directory_entries[i]);
                }
                break;
            case 3:
                classic = !classic;
                printf("Classic mode is now set to: %s\n",
                    classic ? "true" : "false");
                break;
            default:
                printf("\n(!) Invalid input. Please type 1, 2, 3, or q.\n");
                break;
        }
    } while (quit == false);

cleanup:
    arena_free(&wad_arena);
unmap_file:
    unmap_file(&wad_file);
return_from_main:
    return result;
}
