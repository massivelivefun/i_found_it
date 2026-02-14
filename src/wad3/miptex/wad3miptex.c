#include "wad3/miptex/wad3miptex.h"

#include <stdio.h>

#include "utils/endian_utils.h"
#include "utils/file_utils.h"
#include "ifi_errors.h"

//
// Mipmap Textures
//

WAD3MipTex * new_wad3miptex() {
    WAD3MipTex * m = (WAD3MipTex *)malloc(sizeof(WAD3MipTex));
    if (m == NULL) { return NULL; }
    // `m` is valid at this line
    (void)init_wad3miptex(m);
    return m;
}

int init_wad3miptex(WAD3MipTex * m) {
    if (m == NULL) { return IFI_ERROR_NULL_ARGS; }
    // `memset` is valid, WAD3MipTex is the owner of all its data
    (void)memset(m, 0, sizeof(WAD3MipTex));
    return IFI_OK;
}

int init_wad3miptex_from_file(WAD3MipTex * m, FILE * f) {
    if (m == NULL || f == NULL) { return IFI_ERROR_NULL_ARGS; }
    if (read_fixed_string_from_file(f, m->name, MAX_TEXTURE_NAME)) {
        return IFI_ERROR_READ;
    }
    if (read_u32le_from_file(f, &m->width)) { return IFI_ERROR_READ; }
    if (read_u32le_from_file(f, &m->height)) { return IFI_ERROR_READ; }
    for (size_t i = 0; i < MAX_OFFSETS; i += 1) {
        if (read_u32le_from_file(f, &m->offsets[i])) {
            return IFI_ERROR_READ;
        }
    }
    return IFI_OK;
}

int print_wad3miptex(const WAD3MipTex * m) {
    if (m == NULL) { return IFI_ERROR_NULL_ARGS; }
    int len = snprintf(NULL, 0, "\nname: %.16s\nwidth: %d\nheight: %d\n",
        m->name, m->width, m->height);
    if (len < 0) { return IFI_ERROR_ENCODING; }
    size_t total_len = (size_t)len;
    for (size_t i = 0; i < MAX_OFFSETS; i += 1) {
        len = snprintf(NULL, 0, "offset[%zu]: %d\n", i, m->offsets[i]);
        if (len < 0) { return IFI_ERROR_ENCODING; }
        total_len += len;
    }
    char * buffer = (char *)malloc(total_len + 1);
    if (buffer == NULL) { return IFI_ERROR_MEMORY; }
    char * ptr = buffer;
    ptr += sprintf(ptr, "\nname: %.16s\nwidth: %d\nheight: %d\n",
        m->name, m->width, m->height);
    for (size_t i = 0; i < MAX_OFFSETS; i += 1) {
        ptr += sprintf(ptr, "offset[%zu]: %d\n", i, m->offsets[i]);
    }
    printf("%s", buffer);
    free(buffer);
    return IFI_OK;
}

void free_wad3miptex(WAD3MipTex * m) {
    if (m == NULL) { return; }
    free(m);
}
