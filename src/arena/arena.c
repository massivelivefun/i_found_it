#include "utils/arena.h"
#include <stdlib.h>
#include <string.h>

// Helper macro to align memory addresses to 8-byte boundaries
// Example: If offset is 13, this pushes it to 16.
#define ALIGN_UP_8(n) (((n) + 7) & ~7)

bool arena_init(Arena * a, size_t bytes) {
    if (a == NULL || bytes == 0) return false;

    a->base = (uint8_t *)malloc(bytes);
    a->size = bytes;
    a->offset = 0;

    return (a->base != NULL);
}

void * arena_push(Arena * a, size_t bytes) {
    if (a == NULL || a->base == NULL || bytes == 0) {
        return NULL;
    }

    size_t aligned_offset = ALIGN_UP_8(a->offset);

    if (aligned_offset + bytes > a->size) {
        return NULL;
    }

    void * result = a->base + aligned_offset;
    a->offset = aligned_offset + bytes;
    memset(result, 0, bytes);

    return result;
}

size_t arena_save(const Arena * a) {
    if (a == NULL) return 0;
    return a->offset;
}

void arena_restore(Arena * a, size_t saved_offset) {
    if (a != NULL && saved_offset <= a->offset) {
        a->offset = saved_offset;
    }
}

void arena_clear(Arena * a) {
    if (a != NULL) {
        a->offset = 0; 
    }
}

void arena_free(Arena * a) {
    if (a != NULL && a->base != NULL) {
        free(a->base);
        a->base = NULL;
        a->size = 0;
        a->offset = 0;
    }
}
