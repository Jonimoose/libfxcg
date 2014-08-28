#ifndef _FXCG_MINICOMPAT
# error Included without minicompat. Use <fxcg/display.h>
#else

#define LCD_WIDTH_PX 384
#define LCD_HEIGHT_PX 216 

void Bdisp_AreaClr_DD_x3( void*p1 );
void Bdisp_EnableColor( int n );
void Print_OS( unsigned char*msg, int invers, int zero2 );
void Bdisp_PutDisp_DD( void );
void Bdisp_PutDisp_DD_stripe( int y1, int y2 );
void Bdisp_SetPoint_VRAM( int x, int y, int color );
unsigned short Bdisp_GetPoint_VRAM( int x, int y );
void Bdisp_SetPoint_DD( int x, int y, int color );
unsigned short Bdisp_GetPoint_DD_Workbench( int x, int y );
unsigned short Bdisp_GetPoint_DD( int x, int y );
void Bdisp_AllCr_VRAM( void );
void Bdisp_AreaClr( void*p1, unsigned char P2, unsigned short color );
void Cursor_SetFlashOn( unsigned char cursor_type );
void Cursor_SetFlashOff( void );
void Box( int, int, int, int, int );
void AUX_DisplayErrorMessage( int msgno );
void MsgBoxPush( int lines );
void MsgBoxPop( void );
void Box2( int, int );
void locate_OS( int X, int y );
void PrintLine( unsigned char*msg, int imax );
void PrintXY_2( int mode, int x, int y, int msgno, int color );
void PrintXY( int x, int y, char*string, int mode, int color );
void SaveVRAM_1( void );
void LoadVRAM_1( void );
void SetBackGround( int );

#endif /* _FXCG_MINICOMPAT */
