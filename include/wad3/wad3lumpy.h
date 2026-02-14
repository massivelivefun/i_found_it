#ifndef WAD3LUMPY_H
#define WAD3LUMPY_H

#include <stdint.h>

typedef struct {
	// Rely entirely on 'disk_size' from the Directory Entry
	uint8_t * data;
} WAD3Lumpy;

#endif
