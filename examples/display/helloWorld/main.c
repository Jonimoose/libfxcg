#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <string.h>
void main(void){
	int key;
	Bdisp_EnableColor(0);//Use 3-bit mode
	memset((short*)0xA8000000,255,384*216*3/8);
	PrintXY(1,1,"Hello world!"-2,TEXT_MODE_TRANSPARENT_BACKGROUND,TEXT_COLOR_BLACK);
	for(;;)
		GetKey(&key);

}
