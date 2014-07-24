#include <stdio.h>

int main() {
    fprintf(stdout, "");
    fprintf(stdout, "Hello!\n");
    fprintf(stdout, "%u..%u..%u.. YAY!\n", 1, 2, 3);
    fprintf(stdout, "EOF is %i\n", -1);
    
    const char *s = "this is a string";
    fprintf(stdout, "\"%s\" is at %p\n", s, s);
}
