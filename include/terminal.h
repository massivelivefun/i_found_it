#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

typedef struct {
    const uint8_t * data;
    size_t size;
} ValidatedWAD;

bool validate_wad_structure(const uint8_t * file_data, size_t file_size);
int make_dir(char * path);
void clear_terminal();
void print_menu();
int validate_magic(char * magic);
int handle_file_entry_select(uint32_t * number, uint32_t number_of_dirs);

#endif
