#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "file_creation.h"

// --- Test Helper ---
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
             "/tmp/", "Grass", "_Diff", ".png",
             "/tmp/Grass_Diff.png");

    run_test("Empty Suffix",
             "/tmp/", "Grass", "", ".png",
             "/tmp/Grass.png");

    run_test("No Path",
             "", "Grass", "_Diff", ".png",
             "Grass_Diff.png");

    run_test("Complex Ext",
             "/data/", "Map", "", ".tar.gz",
             "/data/Map.tar.gz");

    run_test("NULL Suffix",
             "/tmp/", "Grass", NULL, ".png",
             "/tmp/Grass.png");

    printf("----------------\n"
           "Tests Complete.\n");
    return 0;
}
