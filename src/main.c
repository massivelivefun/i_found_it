#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "wad3miptex.h"
#include "wad3directoryentry.h"
#include "wad3header.h"
#include "wad3pic.h"

#include "file_creation.h"
#include "terminal.h"

int main(int argc, char ** argv) {
    if (argc != 3) {
        fprintf(stderr, "Three args are needed: ./ifi <path/input_file.wad> "
            "<output_path>\n");
        return 1;
    }

    if (make_dir(argv[2]) == 0) {
        printf("Directory created successfully: %s\n", argv[2]);
    } else {
        if (errno == EEXIST) {
            // printf("Directory already exists: %s (leaving it alone)\n", argv[2]);
        } else {
            perror("Error creating directory\n");
            return 1;
        }
    }

    FILE * f = fopen(argv[1], "rb");
    if (f == NULL) {
        fprintf(stderr, "Cannot read file, doesn't exist: %s\n", argv[1]);
        return 1;
    }

    WAD3Header h;
    if (new_wad3header_from_file(&h, f) != 0) {
        fprintf(stderr, "Failed to read header: %s\n", argv[1]);
        fclose(f);
        return 1;
    }
    // print_wad3header(&h);

    int validation_result = validate_magic(h.magic);
    if (validation_result != 0) {
        fprintf(stderr, "Unsupported Magic.\n");
        fclose(f);
        return 1;
    }

    // navigate file to start of directory entry from the start of the file
    fseek(f, h.dir_offset, SEEK_SET);

    WAD3DirectoryEntry directory_entries[h.num_dirs];
    for (size_t i = 0; i < h.num_dirs; i += 1) {
        WAD3DirectoryEntry d;
        if (new_wad3directoryentry_from_file(&d, f) != 0) {
            fprintf(stderr, "Failed to read directory entry: %s\n", argv[1]);
            fclose(f);
            return 1;
        }
        directory_entries[i] = d;
    }

    char choice;
    bool classic = false;

    char menu_buffer[32];

    do {
        print_menu();

        if (fgets(menu_buffer, sizeof(menu_buffer), stdin) == NULL) {
            break; 
        }

        choice = menu_buffer[0];

        switch (choice) {
            case '1': {
                    int result = create_texture(
                        f, argv[1], argv[2], directory_entries, h.num_dirs,
                        classic);
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
            case '3':
                classic = !classic;
                printf("Classic mode is now set to: %s\n",
                    classic ? "true" : "false");
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
