#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "file_creation.h"

void run_test(
    const char *name, 
    const char *out_path, 
    const char *tex_name, 
    const char *suffix, 
    const char *ext, 
    const char *expected
) {
    char *result = NULL;
    printf("Test: %s ... ", name);

    int status = create_single_allocated_output_file_path(
        &result, out_path, tex_name, suffix, ext
    );

    if (status != 0) {
        printf("FAILED (Return Code %d)\n", status);
        return;
    }

    if (strcmp(result, expected) == 0) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n"
               "  Expected: '%s'\n"
               "  Got:      '%s'\n",
               expected, result);
    }

    free(result);
}

int main() {
    printf("Running Tests...\n"
           "----------------\n");

    run_test("Standard",
             "/tmp/", "Grass", "_mm0", ".ppm",
             "/tmp/Grass_mm0.ppm");

    run_test("Empty Suffix",
             "/tmp/", "Water", "", ".ppm",
             "/tmp/Water.ppm");

    run_test("No Path",
             "", "Lava", "_mm1", ".ppm",
             "Lava_mm1.ppm");

    run_test("Complex Ext",
             "/data/", "Map", "", ".tar.gz",
             "/data/Map.tar.gz");

    run_test("NULL Suffix",
             "/tmp/", "Tree", NULL, ".ppm",
             "/tmp/Tree.ppm");

    printf("----------------\n"
           "Tests Complete.\n");
    return 0;
}
