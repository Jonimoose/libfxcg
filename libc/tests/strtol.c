#include <stdio.h>
#include <limits.h>

struct {
    const char *str;
    const int base;
    const long result;
} tests[] = {
    {"   0", 10, 0},
    {"0101", 2, 5},
    {"ffff", 16, 0xffff}
};

int main(int argc, const char **argv) {
    for (int i = 0; i < (sizeof(tests) / sizeof(tests[0])); i++) {
        printf("\"%s\" base %i: ", tests[i].str, tests[i].base);
        char *str_end;
        long v = strtol(tests[i].str, &str_end, tests[i].base);
        if (v != tests[i].result)
            fprintf(stderr, "fail.  Expected %i, got %i\n", tests[i].result, v);
        else
            printf("pass\n");
    }
}
