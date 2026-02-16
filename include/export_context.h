#ifndef EXPORT_CONTEXT_H
#define EXPORT_CONTEXT_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    Arena * arena;
    const uint8_t * file_data;
    const char * output_path;
    const char * texture_name;
    uint32_t entry_offset;
    bool classic_mode;
} ExportContext;

#endif
