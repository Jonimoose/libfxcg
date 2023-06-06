#include <fxcg/display.h>
#include <fxcg/keyboard.h>

#include <calctype/fonts/arial_small/arial_small.h>
#include <calctype/fonts/comic_sans/comic_sans.h>
#include <calctype/fonts/consolas_intl/consolas_intl.h>
#include <calctype/fonts/garamond_large/garamond_large.h>

int main(void) {

  Bdisp_EnableColor(1);

  Bdisp_Fill_VRAM(COLOR_SILVER, 1);

  CalcType_Draw(&arial_small, "Arial Small", 10, 24, COLOR_RED, 0, 0);
  CalcType_Draw(&consolas_intl, "Consolas intl", 10, 39, COLOR_GREEN, 0, 0);
  CalcType_Draw(&comic_sans, "Comic Sans", 10, 50, COLOR_BLUE, 0, 0);
  CalcType_Draw(&garamond_large, "Garamond Large", 10, 65, COLOR_BLACK, 0, 0);

  int key;

  while (1) {
    GetKey(&key);
  }

  return 0;
}
