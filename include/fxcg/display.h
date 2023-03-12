#ifndef __FXCG_DISPLAY_H
#define __FXCG_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_WIDTH_PX 384
#define LCD_HEIGHT_PX 216

typedef unsigned short color_t;

//General display manipulating syscalls:
struct display_fill {
    int x1;
    int y1;
    int x2;
    int y2;
    unsigned char mode;
};
void Bdisp_AreaClr( struct display_fill *area, unsigned char P2, unsigned short color );
void Bdisp_EnableColor( int n );
//Frame control:
void DrawFrame( int color );
unsigned short FrameColor( int mode, unsigned short color );
void DrawFrameWorkbench( int, int, int, int, int );
//VRAM general display manipulating syscalls:
void *GetVRAMAddress(void); // Return a pointer to the system's video memory.
void* GetSecondaryVRAMAddress(void); // Return a pointer to the memory used by SaveVRAM_1 and LoadVRAM_1.
void Bdisp_AllClr_VRAM( void );
void Bdisp_SetPoint_VRAM( int x, int y, int color );
void Bdisp_SetPointWB_VRAM( int x, int y, int color );
unsigned short Bdisp_GetPoint_VRAM( int x, int y );
void SaveVRAM_1( void );
void LoadVRAM_1( void );
void Bdisp_Fill_VRAM( int color, int mode );
//DD display manipulating syscalls:
void Bdisp_AreaClr_DD_x3( void*p1 );
void Bdisp_DDRegisterSelect( int registerno );
void Bdisp_PutDisp_DD( void );
void Bdisp_PutDisp_DD_stripe( int y1, int y2 );
void Bdisp_SetPoint_DD( int x, int y, int color );
unsigned short Bdisp_GetPoint_DD_Workbench( int x, int y );
unsigned short Bdisp_GetPoint_DD( int x, int y );
void DirectDrawRectangle( int x1, int y1, int x2, int y2, unsigned short color );
void HourGlass( void );
void Bdisp_DefineDMARange(int x1, int x2, int y1, int y2);
unsigned short Bdisp_WriteDDRegister3_bit7(int value);

// Graphic writing:
struct display_graph {
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
    char one1;
    int transparency;
};

void Bdisp_WriteGraphVRAM(struct display_graph* gd);
void Bdisp_WriteGraphDD_WB(struct display_graph* gd);

//Shape drawing:
struct display_shape {
    int dx;
    int dy;
    int wx;
    int wy;
    int color;
    struct display_fill saved;
};
void Bdisp_ShapeBase3XVRAM( void*shape );
void Bdisp_ShapeBase( unsigned char*work, struct display_shape *shape, int color, int line_width, int zero1, int zero2 );
void Bdisp_ShapeToVRAM16C( void*, int color );
void Bdisp_ShapeToDD( void*shape, int color );

// The following rectangle-related syscalls draw a rectangle to VRAM, x between 0 and 383 (inclusive), y between 0 and 191 (inclusive).
// These add 24 pixels automatically, avoiding the status area:

// Draws a rectangle to VRAM using a TEXT_COLOR.
void Bdisp_Rectangle( int x1, int y1, int x2, int y2, char color );
// Draws a filled rectangle to VRAM using a TEXT_COLOR
void Bdisp_FilledRectangle( int x1, int y1, int x2, int y2, char color );
// Draws a filled rectangle to VRAM using a color_t
void Bdisp_FilledRectangleFullColor( int x1, int y1, int x2, int y2, unsigned short color );


//Background-related syscalls
void SetBackGround( int );
void WriteBackground( void*target, int width, int height, void*source, int, int, int );

//Message boxes, error messages, dialogs and the like:
void Box( int, int, int, int, int );
void BoxInnerClear( int );
void Box2( int, int );
void BoxYLimits( int lines, int*top, int*bottom );
void AUX_DisplayErrorMessage( int msgno );
void MsgBoxPush( int lines );
void MsgBoxPop( void );
void DisplayMessageBox( unsigned char*message );
short CharacterSelectDialog( void );
unsigned char ColorIndexDialog1( unsigned char initial_index, unsigned short disable_mask );
void MsgBoxMoveWB( void*buffer, int x0, int y0, int x1, int y1, int direction ); //it's more general purpose, works not only for MsgBoxes but for any VRAM contents.

//Cursor manipulating syscalls:
void locate_OS( int X, int y );
void Cursor_SetFlashOn( unsigned char cursor_type );
void Cursor_SetFlashOff( void );
int SetCursorFlashToggle( int );
void Keyboard_CursorFlash( void );

//Character printing syscalls:
enum
{
  TEXT_COLOR_BLACK = 0,
  TEXT_COLOR_BLUE = 1,
  TEXT_COLOR_GREEN = 2,
  TEXT_COLOR_CYAN = 3,
  TEXT_COLOR_RED = 4,
  TEXT_COLOR_PURPLE = 5,
  TEXT_COLOR_YELLOW = 6,
  TEXT_COLOR_WHITE = 7
};

enum
{
  TEXT_MODE_NORMAL = 0x00,
  TEXT_MODE_INVERT = 0x01,
  TEXT_MODE_TRANSPARENT_BACKGROUND = 0x20,
  TEXT_MODE_AND = 0x21
};
void PrintLine(const char *msg, int imax);
void PrintLine2(int, int, const char *, int, int, int, int, int);
void PrintXY_2( int mode, int x, int y, int msgno, int color );
void PrintXY( int x, int y, const char *string, int mode, int color );
void PrintCXY( int, int, const char *, int, int, int, int, int, int );
void PrintGlyph( int, int, unsigned char*glyph, int, int color, int back_color, int );
void*GetMiniGlyphPtr( unsigned short mb_glyph_no, unsigned short*glyph_info );
void PrintMiniGlyph(int x, int y, void*glyph, int mode_flags, int glyph_width, int, int, int, int, int color, int back_color, int );
void PrintMini( int *x, int *y, const char *MB_string, int mode_flags, unsigned int xlimit, int P6, int P7, int color, int back_color, int writeflag, int P11 );
void PrintMiniMini( int *x, int *y, const char *MB_string, int mode1, char color, int mode2 );
void Print_OS( const char*msg, int mode, int zero2 );
void Bdisp_WriteSystemMessage( int x, int y, int msgno, int mode, char color3 );
void Bdisp_MMPrintRef(int*x, int*y, unsigned char*, int mode, int xmax, int, int, int color, int, int, int);
void Bdisp_MMPrint(int x, int y, unsigned char*, int mode, int xmax, int, int, int color, int backcolor, int, int);

enum
{
    PRINT_CHARSET_DEFAULT = 0,
    PRINT_CHARSET_GB18030 = 0x03a8,
};

/**
 * Set the character set used for following text output functions.
 *
 * Known values are provided as PRINT_CHARSET_* constants.
 *
 * Returns 1 if the provided charset code is recognized, 0 otherwise.
 */
int ProcessPrintChars(unsigned short charset);

/** Set the character set to GB18030. */
void EnableGB18030(void);
/** Set the character set to latin. */
void DisableGB18030(void);

//Progressbars and scrollbars:
struct scrollbar
{
  unsigned int I1; // unknown changes indicator height, set to 0
  unsigned int indicatormaximum; // max logical indicator range
  unsigned int indicatorheight; // height of the indicator in units
  unsigned int indicatorpos; // indicator position in units of max
  unsigned int I5; // unknown, set to 0
  unsigned short barleft; // x position of bar
  unsigned short bartop; // y position of bar
  unsigned short barheight; // height of bar
  unsigned short barwidth; // width of bar
} ;
void Scrollbar(struct scrollbar *scrollbar);
void StandardScrollbar( void* );
void ProgressBar(int, int );
void ProgressBar0(int P1, int P2, int P3, int current, int max);
void ProgressBar2(unsigned char *heading, int current, int max);

//Status area/header related syscalls:
// define status area
#define DSA_CLEAR                               0
#define DSA_SETDEFAULT                          1
// status area flags
#define SAF_BATTERY                             0x0001
#define SAF_ALPHA_SHIFT                         0x0002
#define SAF_SETUP_INPUT_OUTPUT                  0x0004
#define SAF_SETUP_FRAC_RESULT                   0x0008
#define SAF_SETUP_ANGLE                         0x0010
#define SAF_SETUP_COMPLEX_MODE                  0x0020
#define SAF_SETUP_DISPLAY                       0x0040
#define SAF_TEXT                                0x0100
#define SAF_GLYPH                               0x0200

int DefineStatusAreaFlags( int mode, int flags, char *color1, char *color2 );
void DefineStatusGlyph( int, void* );
void DefineStatusMessage( char*msg, short P2, char P3, char P4 );
void DisplayStatusArea( void );
void DrawHeaderLine( void );
void EnableStatusArea( int );
void Bdisp_HeaderFill( unsigned char color_idx1, unsigned char color_idx2 );
void Bdisp_HeaderFill2( unsigned int, unsigned int, unsigned char, unsigned char );
void Bdisp_HeaderText( void );
void Bdisp_HeaderText2( void );
void EnableDisplayHeader( int, int );
//Status area icon syscalls: (it may be more appropriate to use the status area flags)
void APP_EACT_StatusIcon( int ); //not sure what this is exactly for, if it displays something on screen it's here, otherwise in app.h. will test some day (gbl08ma)
void SetupMode_StatusIcon( void ); //not sure what this does, doesn't seem to be documented anywhere. will test some day (gbl08ma)
void d_c_Icon( unsigned int );
void BatteryIcon( unsigned int );
void KeyboardIcon( unsigned int );
void LineIcon( unsigned int );
void NormIcon( unsigned int );
void RadIcon( unsigned int );
void RealIcon( unsigned int );

//Other:
void FKey_Display( int, void* );
void GetFKeyPtr( int, void* );
void DispInt( int, int ); //not sure what this does, doesn't seem to be documented anywhere. will test some day (gbl08ma)
int LocalizeMessage1( int msgno, char*result );
int SMEM_MapIconToExt( unsigned char*filename, unsigned short*foldername, unsigned int*msgno, unsigned short*iconbuffer ); // despite starting with SMEM, this is mostly a graphical function used to get icons for different file types.

//Not syscalls (defined within libfxcg):
void VRAM_CopySprite(const color_t* data, int x, int y, int width, int height);
void VRAM_XORSprite(const color_t* data, int x, int y, int width, int height);

// Original Author, Shaun McFall (Merthsoft)
// Used with permission

#define COLOR_ALICEBLUE (color_t)0xF7DF
#define COLOR_ANTIQUEWHITE (color_t)0xFF5A
#define COLOR_AQUA (color_t)0x07FF
#define COLOR_AQUAMARINE (color_t)0x7FFA
#define COLOR_AZURE (color_t)0xF7FF
#define COLOR_BEIGE (color_t)0xF7BB
#define COLOR_BISQUE (color_t)0xFF38
#define COLOR_BLACK (color_t)0x0000
#define COLOR_BLANCHEDALMOND (color_t)0xFF59
#define COLOR_BLUE (color_t)0x001F
#define COLOR_BLUEVIOLET (color_t)0x895C
#define COLOR_BROWN (color_t)0xA145
#define COLOR_BURLYWOOD (color_t)0xDDD0
#define COLOR_CADETBLUE (color_t)0x5CF4
#define COLOR_CHARTREUSE (color_t)0x7FE0
#define COLOR_CHOCOLATE (color_t)0xD343
#define COLOR_CORAL (color_t)0xFBEA
#define COLOR_CORNFLOWERBLUE (color_t)0x64BD
#define COLOR_CORNSILK (color_t)0xFFDB
#define COLOR_CRIMSON (color_t)0xD8A7
#define COLOR_CYAN (color_t)0x07FF
#define COLOR_DARKBLUE (color_t)0x0011
#define COLOR_DARKCYAN (color_t)0x0451
#define COLOR_DARKGOLDENROD (color_t)0xBC21
#define COLOR_DARKGRAY (color_t)0xAD55
#define COLOR_DARKGREEN (color_t)0x0320
#define COLOR_DARKKHAKI (color_t)0xBDAD
#define COLOR_DARKMAGENTA (color_t)0x8811
#define COLOR_DARKOLIVEGREEN (color_t)0x5345
#define COLOR_DARKORANGE (color_t)0xFC60
#define COLOR_DARKORCHID (color_t)0x9999
#define COLOR_DARKRED (color_t)0x8800
#define COLOR_DARKSALMON (color_t)0xECAF
#define COLOR_DARKSEAGREEN (color_t)0x8DF1
#define COLOR_DARKSLATEBLUE (color_t)0x49F1
#define COLOR_DARKSLATEGRAY (color_t)0x2A69
#define COLOR_DARKTURQUOISE (color_t)0x067A
#define COLOR_DARKVIOLET (color_t)0x901A
#define COLOR_DEEPPINK (color_t)0xF8B2
#define COLOR_DEEPSKYBLUE (color_t)0x05FF
#define COLOR_DIMGRAY (color_t)0x6B4D
#define COLOR_DODGERBLUE (color_t)0x1C9F
#define COLOR_FIREBRICK (color_t)0xB104
#define COLOR_FLORALWHITE (color_t)0xFFDE
#define COLOR_FORESTGREEN (color_t)0x2444
#define COLOR_FUCHSIA (color_t)0xF81F
#define COLOR_GAINSBORO (color_t)0xDEFB
#define COLOR_GHOSTWHITE (color_t)0xFFDF
#define COLOR_GOLD (color_t)0xFEA0
#define COLOR_GOLDENROD (color_t)0xDD24
#define COLOR_GRAY (color_t)0x8410
#define COLOR_GREEN (color_t)0x0400
#define COLOR_GREENYELLOW (color_t)0xAFE5
#define COLOR_HONEYDEW (color_t)0xF7FE
#define COLOR_HOTPINK (color_t)0xFB56
#define COLOR_INDIANRED (color_t)0xCAEB
#define COLOR_INDIGO (color_t)0x4810
#define COLOR_IVORY (color_t)0xFFFE
#define COLOR_KHAKI (color_t)0xF731
#define COLOR_LAVENDER (color_t)0xE73F
#define COLOR_LAVENDERBLUSH (color_t)0xFF9E
#define COLOR_LAWNGREEN (color_t)0x7FE0
#define COLOR_LEMONCHIFFON (color_t)0xFFD9
#define COLOR_LIGHTBLUE (color_t)0xAEDC
#define COLOR_LIGHTCORAL (color_t)0xF410
#define COLOR_LIGHTCYAN (color_t)0xE7FF
#define COLOR_LIGHTGOLDENRODYELLOW (color_t)0xFFDA
#define COLOR_LIGHTGRAY (color_t)0xD69A
#define COLOR_LIGHTGREEN (color_t)0x9772
#define COLOR_LIGHTPINK (color_t)0xFDB8
#define COLOR_LIGHTSALMON (color_t)0xFD0F
#define COLOR_LIGHTSEAGREEN (color_t)0x2595
#define COLOR_LIGHTSKYBLUE (color_t)0x867F
#define COLOR_LIGHTSLATEGRAY (color_t)0x7453
#define COLOR_LIGHTSTEELBLUE (color_t)0xB63B
#define COLOR_LIGHTYELLOW (color_t)0xFFFC
#define COLOR_LIME (color_t)0x07E0
#define COLOR_LIMEGREEN (color_t)0x3666
#define COLOR_LINEN (color_t)0xFF9C
#define COLOR_MAGENTA (color_t)0xF81F
#define COLOR_MAROON (color_t)0x8000
#define COLOR_MEDIUMAQUAMARINE (color_t)0x6675
#define COLOR_MEDIUMBLUE (color_t)0x0019
#define COLOR_MEDIUMORCHID (color_t)0xBABA
#define COLOR_MEDIUMPURPLE (color_t)0x939B
#define COLOR_MEDIUMSEAGREEN (color_t)0x3D8E
#define COLOR_MEDIUMSLATEBLUE (color_t)0x7B5D
#define COLOR_MEDIUMSPRINGGREEN (color_t)0x07D3
#define COLOR_MEDIUMTURQUOISE (color_t)0x4E99
#define COLOR_MEDIUMVIOLETRED (color_t)0xC0B0
#define COLOR_MIDNIGHTBLUE (color_t)0x18CE
#define COLOR_MINTCREAM (color_t)0xF7FF
#define COLOR_MISTYROSE (color_t)0xFF3C
#define COLOR_MOCCASIN (color_t)0xFF36
#define COLOR_NAVAJOWHITE (color_t)0xFEF5
#define COLOR_NAVY (color_t)0x0010
#define COLOR_OLDLACE (color_t)0xFFBC
#define COLOR_OLIVE (color_t)0x8400
#define COLOR_OLIVEDRAB (color_t)0x6C64
#define COLOR_ORANGE (color_t)0xFD20
#define COLOR_ORANGERED (color_t)0xFA20
#define COLOR_ORCHID (color_t)0xDB9A
#define COLOR_PALEGOLDENROD (color_t)0xEF55
#define COLOR_PALEGREEN (color_t)0x9FD3
#define COLOR_PALETURQUOISE (color_t)0xAF7D
#define COLOR_PALEVIOLETRED (color_t)0xDB92
#define COLOR_PAPAYAWHIP (color_t)0xFF7A
#define COLOR_PEACHPUFF (color_t)0xFED7
#define COLOR_PERU (color_t)0xCC27
#define COLOR_PINK (color_t)0xFE19
#define COLOR_PLUM (color_t)0xDD1B
#define COLOR_POWDERBLUE (color_t)0xB71C
#define COLOR_PURPLE (color_t)0x8010
#define COLOR_RED (color_t)0xF800
#define COLOR_ROSYBROWN (color_t)0xBC71
#define COLOR_ROYALBLUE (color_t)0x435C
#define COLOR_SADDLEBROWN (color_t)0x8A22
#define COLOR_SALMON (color_t)0xFC0E
#define COLOR_SANDYBROWN (color_t)0xF52C
#define COLOR_SEAGREEN (color_t)0x2C4A
#define COLOR_SEASHELL (color_t)0xFFBD
#define COLOR_SIENNA (color_t)0xA285
#define COLOR_SILVER (color_t)0xC618
#define COLOR_SKYBLUE (color_t)0x867D
#define COLOR_SLATEBLUE (color_t)0x6AD9
#define COLOR_SLATEGRAY (color_t)0x7412
#define COLOR_SNOW (color_t)0xFFDF
#define COLOR_SPRINGGREEN (color_t)0x07EF
#define COLOR_STEELBLUE (color_t)0x4416
#define COLOR_TAN (color_t)0xD5B1
#define COLOR_TEAL (color_t)0x0410
#define COLOR_THISTLE (color_t)0xDDFB
#define COLOR_TOMATO (color_t)0xFB08
#define COLOR_TURQUOISE (color_t)0x471A
#define COLOR_VIOLET (color_t)0xEC1D
#define COLOR_WHEAT (color_t)0xF6F6
#define COLOR_WHITE (color_t)0xFFFF
#define COLOR_WHITESMOKE (color_t)0xF7BE
#define COLOR_YELLOW (color_t)0xFFE0
#define COLOR_YELLOWGREEN (color_t)0x9E66

#ifdef __cplusplus
}
#endif

#endif /* __FXCG_DISPLAY_H */
