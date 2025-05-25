#include <stdio.h>

void report_failed(int failed) {
    if (failed) {
        printf("%d tests failed.\n", failed);
        return;
    } 
    printf("All tests passed.\n");
}

int check_input(int err, const char *input_str) {
    if (err) {
        printf("Error parsing input '%s'\n", input_str);
        return 1;
    }
    return 0;
}
