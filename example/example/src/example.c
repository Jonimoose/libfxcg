#include <display_syscalls.h>
#include <keyboard_syscalls.h>
 
void main(void) {
	int key;
    char blah[10];
    blah[1]='k';
     
	while (1) {
        Bdisp_AllClr_VRAM();
        GetKey(&key);
		switch (key) {
		}
	}
 
	return;
}