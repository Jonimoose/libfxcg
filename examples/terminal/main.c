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
	puts("\x9bm");
	GetKey(&key);
	for(i=0;i<16;++i)
		fputs(stdout,"\x9b""A");
	putchar('\f');
	setcol(0);
	puts("\x9b""30m\x9b""47mBlack\x9b""0m");
	puts("\x9b""31mRed");
	puts("\x9b""32mGreen");
	puts("\x9b""33mYellow");
	puts("\x9b""34mBlue");
	puts("\x9b""35mMagenta");
	puts("\x9b""36mCyan");
	puts("\x9b""37mWhite");
	puts("\x9b""39mDefault");
	puts("Press the menu key to exit");
	while(1)
		GetKey(&key);
}
