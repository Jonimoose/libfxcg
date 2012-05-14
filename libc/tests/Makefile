strtol: strtol.c stdlib.o
	gcc -g -o strtol strtol.c stdlib.o -std=c99 -fno-builtin

stdlib.o: ../stdlib.c
	gcc -g -c -o stdlib.o ../stdlib.c -nostdlib -fno-builtin
