#ifndef FILE_UTILS_H
#define FILE_UTILS_H

// #define bool char

/**
 * Reads a fixed-length string from a file and ensures it is null-terminated.
 * @param f The file stream.
 * @param buffer The destination char array.
 * @param size The physical size in the file (e.g., 16 for WAD names).
 * @param buffer_size The actual size of your C-array (should be size + 1).
 */
static inline int read_fixed_string_from_file(
    FILE * f,
    char * buffer,
    size_t size
) {
    if (f == NULL || buffer == NULL) {
        return 1;
    }
    if (size == 0) {
        return 0;
    }
    if (fread(buffer, size, 1, f) != 1) {
        return 1;
    }
    buffer[size - 1] = '\0';
    return 0;
}

static inline int write_fixed_string_to_file(
    FILE * f,
    const char * str,
    size_t size
) {
    if (f == NULL || str == NULL) { return 1; }
    char padding_buf[size];
    memset(padding_buf, 0, size);
    strncpy(padding_buf, str, size);
    if (fwrite(padding_buf, size, 1, f) != 1) {
        return 1;
    }
    return 0;
}

static inline int read_char_from_file(FILE * f, char * ch) {
    if (f == NULL || ch == NULL) {
        return 1;
    }
    if (fread(ch, sizeof(char), 1, f) != 1) {
        return 1;
    }
    return 0;
}

static inline int read_i8_from_file(FILE * f, int8_t * val) {
    if (f == NULL || val == NULL) {
        return 1;
    }
    if (fread(val, sizeof(int8_t), 1, f) != 1) {
        return 1;
    }
    return 0;
}

// Use this if bool is defined as char
// static inline int read_bool8_from_file(FILE * f, bool * b) {
//     if (f == NULL || b == NULL) {
//         return 1;
//     }
//     uint8_t temp;
//     if (fread(&temp, sizeof(uint8_t), 1, f) != 1) {
//         return 1;
//     }
//     *b = (temp != 0);
//     return 0;
// }

#endif
