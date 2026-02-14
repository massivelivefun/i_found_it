#include "wad3/wad3image.h"

int new_wad3image(
    WAD3MipTex * m,
    uint8_t * indices,
    uint8_t * rgb_data,
    uint8_t dimension,
    WAD3Image * i
) {
    if (m == NULL || indices == NULL || rgb_data == NULL || i == NULL) {
        fprintf(stderr, "Failed to make wad3image.\n");
        return 1;
    }
    WAD3Image image = {
        .width = m->width / dimension,
        .height = m->height / dimension,
        .indices = indices,
        .rgb_data = rgb_data,
    };
    strncpy(image.name, m->name, 16);
    image.name[15] = '\0';
    *i = image;
    return 0;
}

int new_wad3image_from_wad3pic(WAD3Pic * pic, char * name, uint8_t dimension, WAD3Image * i) {
    if (pic == NULL || i == NULL) {
        fprintf(stderr, "Failed to make wad3image from wad3pic.\n");
        return 1;
    }
    WAD3Image image = {
        .width = pic->width / dimension,
        .height = pic->height / dimension,
        .indices = pic->indices,
        .rgb_data = pic->rgb_data,
    };
    strncpy(image.name, name, 16);
    image.name[15] = '\0';
    *i = image;
    return 0;
}
