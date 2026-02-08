#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

int make_dir(char * path);
void clear_terminal();
int validate_magic(char * magic);
int handle_file_entry_select(uint32_t * number, uint32_t number_of_dirs);

#endif
