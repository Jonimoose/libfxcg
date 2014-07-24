strtol: strtol.c stdlib.o
	gcc -g -o strtol strtol.c stdlib.o -std=c99 -fno-builtin

stdlib.o: ../stdlib.c
	gcc -g -c -o stdlib.o ../stdlib.c -fno-builtin

printf: printf.c printf.o
	gcc -m32 -std=gnu99 -g -o printf printf.c printf.o -fno-builtin

printf.o: ../printf.c
	gcc -m32 -std=gnu99 -g -c -o printf.o ../printf.c -fno-builtin
