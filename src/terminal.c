#include "terminal.h"

bool validate_wad_structure(const uint8_t * file_data, size_t file_size) {
    if (file_size < 12) { return false; }

    uint32_t num_dirs = *(uint32_t *)(file_data + 4);
    uint32_t dir_offset = *(uint32_t *)(file_data + 8);

    size_t dir_end_byte = dir_offset + (num_dirs * 32);
    if (dir_end_byte > file_size) {
        return false;
    }

    // We now know it's safe to loop through the directories.
    // Validate every single asset's bounds
    for (size_t i = 0; i < num_dirs; i++) {
        // Calculate where this specific directory entry starts
        const uint8_t * entry_ptr = file_data + dir_offset + (i * 32);
        
        // Extract the offset and compressed size of the actual image data
        uint32_t filepos = *(uint32_t *)(entry_ptr + 0);
        uint32_t disksize = *(uint32_t *)(entry_ptr + 4);

        // Does this image data reach past the end of the file?
        if (filepos + disksize > file_size) {
            return false;
        }
    }

    return true;
}

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

void print_menu() {
    printf(
        "\n========================\n"
        "       I FOUND IT       \n"
        "========================\n"
        " 1. Create a texture\n"
        " 2. Print all directory entries in wad file\n"
        " 3. Toggle classic output on or off\n"
        " q. Quit\n"
        "------------------------\n"
        "Enter selection: "
    );
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
