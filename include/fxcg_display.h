/* Casio fx-CG10/20 display defines and syscalls. */
#ifndef __FXCG_DISPLAY_H__
#define __FXCG_DISPLAY_H__

#include "color.h"

#define LCD_WIDTH_PX 384
#define LCD_HEIGHT_PX 216 

// define status area
#define DSA_CLEAR               0
#define DSA_SETDEFAULT          1
// status area flags
#define SAF_BATTERY             0x0001
#define SAF_ALPHA_SHIFT         0x0002
#define SAF_SETUP_INPUT_OUTPUT  0x0004
#define SAF_SETUP_FRAC_RESULT   0x0008
#define SAF_SETUP_ANGLE         0x0010
#define SAF_SETUP_COMPLEX_MODE  0x0020
#define SAF_SETUP_DISPLAY       0x0040
#define SAF_TEXT                0x0100
#define SAF_GLYPH               0x0200

//
struct TBdispFillArea {
    int x1;
    int y1;
    int x2;
    int y2;
    unsigned char mode;
};

//
struct TShapeProps{
    int dx;
    int dy;
    int wx;
    int wy;
    int color;
    TBdispFillArea saved;
};

#endif
