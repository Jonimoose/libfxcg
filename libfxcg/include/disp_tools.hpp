#ifndef __fxCG_DISPLAY_HPP__
#define __fxCG_DISPLAY_HPP__

// define status area
#define DSA_CLEAR				0
#define DSA_SETDEFAULT			1
// status area flags
#define SAF_BATTERY				0x0001
#define SAF_ALPHA_SHIFT			0x0002
#define SAF_SETUP_INPUT_OUTPUT	0x0004
#define SAF_SETUP_FRAC_RESULT	0x0008
#define SAF_SETUP_ANGLE			0x0010
#define SAF_SETUP_COMPLEX_MODE	0x0020
#define SAF_SETUP_DISPLAY		0x0040
#define SAF_TEXT				0x0100
#define SAF_GLYPH				0x0200

// printmini-flags
#define PMF_ALLOW_STATUSAREA 0x40
#define PMF_INVERT 0x04
#define PMF_DO_NOT_USE_BACKCOLOR 0x02
#define PMF_UPPER_X_LIMIT_IGNORED 0xFFFFFFFF

// boxlimits
#define BOX_XMIN 0x23
#define BOX_XMAX 0x15D

// MessageDlg
enum TMsg { mtWarning, mtError, mtInformation, mtConfirmation, mtDebugInt, mtDebugStr };
enum TMsgRes { mrNONE, mrCANCEL, mrYES, mrNO, mrOK };

// EditDialog
enum TEditDialog { edHEX, edSTR, edHEXSTR };

typedef struct{
	unsigned char*pretext;
	unsigned int value;
	unsigned int maxlen;
} TedHEX;

typedef struct{
	unsigned char*pretext;
	unsigned char*buffer;
	unsigned int maxlen;
} TedSTR;

typedef struct{
	int x1;
	int y1;
	int x2;
	int y2;
	char f[4];
	int patt1;
	int patt2;
} TShape;

typedef struct{
	unsigned short width;
	unsigned short height;
	unsigned short unknown1;
	unsigned short unknown2;
	unsigned short unknown3;
	unsigned short unknown4;
	void*bitmap;
} TBGRbitmap;

typedef struct{
    int x;
    int y;
    int xofs;
    int yofs;
    int width;
    int height;
    char colormode;
    char zero4;
    char P20_1;
    char P20_2;
    int bitmap;
    char color_idx1;
    char color_idx2;
    char color_idx3;
    char P20_3;
    char writemodify;
    char writekind;
    char zero6;
    char one2;
    int transparency;
} TDispGraph;

typedef struct{
	unsigned int I1;
	unsigned int indicatormaximum;
	unsigned int indicatorheight;
	unsigned int indicatorpos;
	unsigned int I5;
	unsigned short barleft;
	unsigned short bartop;
	unsigned short barheight;
	unsigned short barwidth;
} TScrollbar;


typedef unsigned short TStandardScrollbar[0x12];
  
void Print( unsigned char*msg );
void SetStatusMessage( char*msg, char short_color = 4 );
void StatusArea( char*msg, char short_color = 4 );
TMsgRes MessageDlg( void*Msg, TMsg msgtyp, int height = 3 );
int PrintMiniFix( int x, int y, const char*Msg, const int flags, const short color, const short bcolor );
TMsgRes EditDialog( TEditDialog edlg, void*edlgwork );

#include "display_syscalls.h"

#endif