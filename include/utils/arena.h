#ifndef IFI_ARENA_H
#define IFI_ARENA_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t * base;
    size_t size;
    size_t offset;
} Arena;

bool arena_init(Arena * a, size_t bytes);
void * arena_push(Arena * a, size_t bytes);
size_t arena_save(const Arena * a);
void arena_restore(Arena * a, size_t saved_offset);
void arena_clear(Arena * a);
void arena_free(Arena * a);

#endif
