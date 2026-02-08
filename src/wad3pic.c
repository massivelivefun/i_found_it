#include "wad3pic.h"

int new_wad3pic(FILE * f, WAD3Pic * p) {
	if (f == NULL || p == NULL) {
		return 1;
	}
	if (fread(&(p->width), sizeof(uint32_t), 1, f) < 1) {
		return 1;
	}
	if (fread(&(p->height), sizeof(uint32_t), 1, f) < 1) {
		return 1;
	}
	p->indices = (uint8_t *)malloc(sizeof(uint8_t) * (p->width * p->height));
	if (p->indices == NULL) {
		return 1;
	}
	if (fread(&(p->indices), sizeof(uint8_t), (p->width * p->height), f) < (p->width * p->height)) {
		return 1;
	}
	if (fread(&(p->palette_size), sizeof(uint16_t), 1, f) < 1) {
		return 1;
	}
	p->rgb_data = (uint8_t *)malloc(sizeof(uint8_t) * (3 * p->palette_size));
	if (p->rgb_data == NULL) {
		return 1;
	}
	if (fread(&(p->rgb_data), sizeof(uint8_t), 3 * p->palette_size, f) < 3 * p->palette_size) {
		return 1;
	}
	return 0;
}

void print_wad3pic(WAD3Pic * p) {
	printf("\n");
	printf("width: %d\n", p->width);
	printf("height: %d\n", p->height);
	for (size_t i = 0; i < (p->width * p->height); i += 1) {
        printf("indices[%zu]: %d\n", i, p->indices[i]);
    }
	printf("palette_size: %d\n", p->palette_size);
	for (size_t i = 0; i < (3 * p->palette_size); i += 1) {
        printf("indices[%zu]: %d\n", i, p->rgb_data[i]);
    }
}
