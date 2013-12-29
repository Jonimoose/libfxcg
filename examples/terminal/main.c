#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/terminal.h>
#include <stdio.h>
static void setcol(int c){
	printf("\x9b%dm",c);
}
static const char *ColorfulMessage="Colorful";
void main(void){
	FXCGinitTerminal
	puts("This is a demonstration of the Fxcg termianl emulator");
	puts("The goal is xterm compatibility");
	puts("This demo shows off the color and cursor features that are currently implemented");
	int key;
	GetKey(&key);
	puts("\f\0337Saved cursor");
	puts("Press any key to continue");
	GetKey(&key);
	puts("\0338Cursor restored");
	GetKey(&key);
	putchar('\f');
	puts("Colors 30-37 and 40-47");
	int i;
	for(i=40;i<=47;++i){
		setcol(i);
		putchar(' ');
	}
	putchar('\n');
	for(i=0;i<=7;++i){
		setcol((7-i)+30);
		setcol(i+40);
		putchar(ColorfulMessage[i]);
	}
	putchar('\n');
	GetKey(&key);
	putchar('\f');
	setcol(0);
	puts("\x9b""34mBlue");
	puts("Press the menu key to exit");
	while(1)
		GetKey(&key);
}
