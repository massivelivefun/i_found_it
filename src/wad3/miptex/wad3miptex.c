#include "wad3/miptex/wad3miptex.h"

#include <stdio.h>

#include "utils.h"
#include "ifi_errors.h"

int init_wad3miptex_from_data(WAD3MipTex * m, const uint8_t * data) {
    if (m == NULL || data == NULL) { return IFI_ERROR_NULL_ARGS; }
    memcpy(m->name, data, MAX_TEXTURE_NAME);
    m->width = unsafe_read_u32le_from_ptr(data + 16);
    m->height = unsafe_read_u32le_from_ptr(data + 20);
    for (size_t i = 0; i < MAX_OFFSETS; i += 1) {
        size_t idx = 24 + (i * sizeof(uint32_t));
        m->offsets[i] = unsafe_read_u32le_from_ptr(data + idx);
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
