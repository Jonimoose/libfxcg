#include <fxcg/display.h>
#include <fxcg/keyboard.h>
static int keyPressed(int basic_keycode){
    const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
    int row, col, word, bit;
    row = basic_keycode%10;
    col = basic_keycode/10-1;
    word = row>>1;
    bit = col + ((row&1)<<3);
    return (0 != (keyboard_register[word] & 1<<bit));
}
void main(void){
    Bdisp_EnableColor(0);//Use 3-bit mode.
    Bdisp_AllClr_VRAM();
    Bdisp_PutDisp_DD();
    for(;;){
        HourGlass();
        if(keyPressed(KEY_PRGM_MENU)){
            int key;
            GetKey(&key);
        }
    }
}
