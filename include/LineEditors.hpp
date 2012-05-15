#ifndef __fxCG_LINEEDITORS_HPP__
#define __fxCG_LINEEDITORS_HPP__

#define KEY_HELP KEY_CTRL_F1
#define KEY_LEFT KEY_CTRL_LEFT
#define KEY_RIGHT KEY_CTRL_RIGHT
#define KEY_0 KEY_CHAR_0
#define KEY_1 KEY_CHAR_1
#define KEY_2 KEY_CHAR_2
#define KEY_3 KEY_CHAR_3
#define KEY_4 KEY_CHAR_4
#define KEY_5 KEY_CHAR_5
#define KEY_6 KEY_CHAR_6
#define KEY_7 KEY_CHAR_7
#define KEY_8 KEY_CHAR_8
#define KEY_9 KEY_CHAR_9
#define KEY_A KEY_CTRL_XTT
#define KEY_B KEY_CHAR_LOG
#define KEY_C KEY_CHAR_LN
#define KEY_D KEY_CHAR_SIN
#define KEY_E KEY_CHAR_COS
#define KEY_F KEY_CHAR_TAN
#define KEY_EXIT KEY_CTRL_EXIT
#define KEY_EXE KEY_CTRL_EXE

enum TEditMode { emANY, emINTEGER, emHEX };

int InputAny( unsigned char*buffer, int x, int y, int posmax, int&xpos, TEditMode edMode = emANY );

#endif
