#include <stdio.h>
#include <ctype.h>

int main(int argc, const char **argv) {
    printf("isalnum('a') = %i\n", isalnum('a'));
	printf("isalnum('A') = %i\n", isalnum('A'));
	printf("isalnum('z') = %i\n", isalnum('z'));
	printf("isalnum('Z') = %i\n", isalnum('Z'));
	printf("isalnum('g') = %i\n", isalnum('g'));
	printf("isalnum('0') = %i\n", isalnum('0'));
	printf("isalnum('9') = %i\n", isalnum('9'));
	printf("isalnum('5') = %i\n", isalnum('5'));
	printf("isalnum(5) = %i\n", isalnum(5));
	printf("isalnum('\t') = %i\n", isalnum('\t'));
}
