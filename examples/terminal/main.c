#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/terminal.h>
#include <stdio.h>
void setcol(int c){
	printf("\x9b%dm",c);
}
const char *ColorfulMessage="Colorful";
void main(void){
	FXCGinitTerminal
	puts("\0337Saved cursor");
	puts("Press any key to continue");
	int key;
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
	puts("\x9b""34m Blue");
	puts("Press the menu key to exit");
	while(1)
		GetKey(&key);
}
