#include "terminal.h"

int make_dir(char * path) {
    int result;
#ifdef _WIN32
    result = _mkdir(path);
#else
    result = mkdir(path, 0777);
#endif
    return result;
}

void clear_terminal() {
#if _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int validate_magic(char * magic) {
	char usable_magic[5];
    strncpy(usable_magic, magic, 4);
    // for (size_t i = 0; i < 4; i += 1) {
    //     usable_magic[i] = h.magic[i];
    // }
    usable_magic[4] = '\0';

    // Confirm the header and see what the magic is
    if (strcmp(usable_magic, "WAD3") == 0) {
        // Half Life 1, (Goldsrc) maybe?
        // This is allowed
        // printf("WAD3 is supported.\n");
        return 0;
    } else if (strcmp(usable_magic, "WAD2") == 0) {
        // Quake
        fprintf(stderr, "WAD2 not supported.\n");
        return 1;
    } else if (strcmp(usable_magic, "WADF") == 0) {
        // Internal asset management
        fprintf(stderr, "WADF not supported.\n");
        return 1;
    } else if (strcmp(usable_magic, "BSPX") == 0) {
        // WAD file packed into an BSP file
        fprintf(stderr, "BSPX not supported.\n");
        return 1;
    } else if (strcmp(usable_magic, "IWAD") == 0) {
        // Original doom master file with all the original assets
        // e.g. doom.wad, or doom2.wad
        fprintf(stderr, "IWAD not supported.\n");
        return 1;
    } else if (strcmp(usable_magic, "PWAD") == 0) {
        // Patch mods that overwrite the file 
        fprintf(stderr, "PWAD not supported.\n");
        return 1;
    } else {
        printf("Magic not recognized: %4s\n", usable_magic);
        return 1;
    }
    return 0;
}

int handle_file_entry_select(uint32_t * number, uint32_t number_of_dirs) {
    char input_buffer[128];
    char *end_ptr;
    long choice_val;

    printf("Enter an index shown in option #2: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return 1; 
    }

    input_buffer[strcspn(input_buffer, "\n")] = 0;

    choice_val = strtol(input_buffer, &end_ptr, 10);

    // Reject if:
    // The string was empty or contained no digits (end_ptr == input_buffer)
    // There were non-digit characters after the number (*end_ptr != '\0')
    if (end_ptr == input_buffer || *end_ptr != '\0') {
        fprintf(stderr,
            "Invalid input: '%s'. Please enter digits only.\n", input_buffer);
        return 1;
    }

    if (choice_val < 1 || choice_val > (long)number_of_dirs) {
        fprintf(stderr,
            "Error: Number out of range (1 to %u).\n", number_of_dirs);
        return 1;
    }

    *number = (uint32_t)choice_val;
    return 0;
}
