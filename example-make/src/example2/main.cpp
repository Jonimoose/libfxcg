#include <fxcg/display.h>
#include <fxcg/keyboard.h>

int main()
{
  int key;
  
  PrintXY(1,1,"  Hello World!", TEXT_MODE_NORMAL, TEXT_COLOR_RED);
  
  while(1) GetKey(&key);
}

