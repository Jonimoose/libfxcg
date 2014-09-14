#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <string.h>
void main(void){
    int key;
    Bdisp_EnableColor(0);//Use 3-bit mode
    Bdisp_AllClr_VRAM();
    PrintXY(1,1,"  Hello world!",TEXT_MODE_TRANSPARENT_BACKGROUND,TEXT_COLOR_BLACK);
    for(;;)
        GetKey(&key);

}
