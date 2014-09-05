#include <fxcg/display.h>
#include <fxcg/keyboard.h>
static void getStrn(int x,int y,int n,char*buffer){
	int start = 0; // Used for scrolling left and right
	int cursor = 0; // Cursor position
	buffer[0] = '\0'; // This sets the first character to \0, also represented by "", an empty string
	DisplayMBString((unsigned char*)buffer, start, cursor, x, y); // Last to parameters are X,Y coords (not in pixels)
	int key;
	while(1){
		GetKey(&key); // Blocking is GOOD.  This gets standard keys processed and, possibly, powers down the CPU while waiting
		if(key == KEY_CTRL_EXE){
			// Ok
			break;
		}else if(key == KEY_CTRL_EXIT){
			// Aborted
			break;
		}
		if(key && key < 30000){
			cursor = EditMBStringChar((unsigned char*)buffer, n, cursor, key);
			DisplayMBString((unsigned char*)buffer, start, cursor, x,y);
		}else{
			EditMBStringCtrl((unsigned char*)buffer, n, &start, &cursor, &key, x, y);
		}
	}
}
static void askQ(const char*msg,char*buf,int n){
	//Clear areas from old question.
	struct display_fill fill;
	fill.x1=0;
	fill.x2=383;
	fill.y1=144;
	fill.y2=168;
	fill.mode=0;
	Bdisp_AreaClr(&fill,1,0);
	PrintXY(1,6,msg,0,0);
	getStrn(1,7,n,buf);
}
void main(void){
	int x,y;
	char buf[1024];//In this example nothing is done with the buffer, I leave it as an exercise to the reader to do something with the inputted text.
	Bdisp_EnableColor(0);
	Bdisp_AllClr_VRAM();
	//Print the "explanation"
	PrintXY(1,1,"  Stop!",0,0);
	x=0;
	y=32;
	PrintMini(&x,&y,"Who would cross the Bridge of Death",0,0xFFFFFFFF,0,0,0,0xFFFF,1,0);
	x=0;
	y=56;
	PrintMini(&x,&y,"must answer me these questions three,",0,0xFFFFFFFF,0,0,0,0xFFFF,1,0);
	x=0;
	y=80;
	PrintMini(&x,&y,"ere the other side he see.",0,0xFFFFFFFF,0,0,0,0xFFFF,1,0);
	PrintXY(1,5,"  What... is",0,0);
	for(;;){
		askQ("  your name?",buf,1024);
		askQ("  your quest?",buf,1024);
		askQ("  your favorite color?",buf,1024);//The text fits by using American spelling. Yes I know Monty Python is a British show.
	}
}
