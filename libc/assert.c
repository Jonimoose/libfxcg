#include <fxcg/display.h>
#include <fxcg/keyboard.h>
void assert(int ex){
	if(!ex){
		int x=0;
		int y=0;
		PrintMini(&x,&y,"assert",0,0xFFFFFFFF,0,0,0,0xFFFF,1,0);
		int key;
		while(1);
			GetKey(&key);
	}
}
