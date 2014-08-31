#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <string.h>
/* Note that a progress bar has the side effect of making the long calculation you are doing to take longer.
 * The reason for this is because the screen needs to be redrawn.
 * Try figuring out an optimal balance between progress bar responsiveness and how many times it is updated.
 * A few times a second should be good enough.
 * This example does not do any long calculations like a real program would.
 * Instead it just redraws the progress bar a lot to make it take awhile thus demonstrating my point about limiting how often to redraw the progress bar.
 * Also consider: "Why do I need a progress bar? Can my code be written to be faster removing the need for a progress bar?".
 * Really the only reason you would need a progress bar is if the calcualtion takes lots of time.
 * Two seconds as an example without a progress bar should be fine.
 */
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
	Bdisp_EnableColor(0);//Use 3-bit mode. The progress bar looks the same in 16-bit mode.
	Bdisp_AllClr_VRAM();
	for(;;){
		int i;
		for(i=0;i<9001;++i){
			ProgressBar2("Acting busy",i,9001);
			if(keyPressed(KEY_PRGM_MENU)){
				int key;
				GetKey(&key);
				break;
			}
		}
		MsgBoxPop();
	}
}
