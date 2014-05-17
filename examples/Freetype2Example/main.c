#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/terminal.h>
#include <stdio.h>
#include <ft2build.h>
#include "Georgia.h"
#include FT_FREETYPE_H
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H
FT_Library ft;
FT_Face face; /* handle to face object */
static void errorMessage(int line,int error){
	fprintf(stderr,"Error line: %d code %d\nPress the MENU key to exit\n",line,error);
	int keyW;
	while(1) GetKey(&keyW);
}
static void graySpans(int y,int count,const FT_Span *spans,void *user){
	int pen_y=user;
	pen_y&=0xFFFF;
	int pen_x=user;
	pen_x>>=16;
	//printf("pen_x: %d pen_y: %d spans->x: %d y: %d spans->len: %d spans->coverage: %d\n",pen_x,pen_y,spans->x,y,spans->len,spans->coverage);
	y=-y;
	int i;
	for(i=0;i<count;++i){
		const FT_Span span = spans[i];
		unsigned short *o=(unsigned short *)0xA8000000;
		o+=(y*384)+(pen_y*384)+span.x+pen_x;
		int a=span.len;
		unsigned short c=span.coverage;
		c=((c&248)<<8)|((c&252)<<3)|((c&248)>>3);
		while(a--)
			*o++=c;
	}
	//int keyW;
	//GetKey(&keyW);
}
static int drawCstring(const char * str,int pen_x,int pen_y){
	int len=strlen(str),i,error;
	FT_GlyphSlot slot = face->glyph; /* a small shortcut */
	for(i=0;i<len;++i){

		int glyph_index = FT_Get_Char_Index(face,str[i]);

		int error = FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT);
		if(error) errorMessage(__LINE__,error);

		FT_Raster_Params params;

		params.target = 0;
		params.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA;
		params.user = pen_y|(pen_x<<16);
		params.gray_spans = &graySpans;
		params.black_spans = 0;
		params.bit_set = 0;
		params.bit_test = 0;

		FT_Outline* outline = &face->glyph->outline;

		FT_Outline_Render(ft,outline,&params);

		/* increment pen position */
		pen_x += slot->advance.x >> 6;
	}
	return pen_x;
}
void main(void){
	FXCGinitTerminal
	int key;
	int error = FT_Init_FreeType(&ft);
	if (error) errorMessage(__LINE__,error);
	error = FT_New_Memory_Face(ft, Georgia, /* first byte in memory */ 174299, /* size in bytes */ 0, /* face_index */ &face );
	if (error) errorMessage(__LINE__,error);
	/* The casio prizm has about a 2.8x1.6 inch display NOTE the ruler I used is not the greatest
	So therefore the casio prizm has 384/2.8=137 (rounded to nearst integer) dpi width and 216/1.6=135 dpi height*/
	error = FT_Set_Char_Size( face, /* handle to face object */ 0, /* char_width in 1/64th of points */ 26*64, /* char_height in 1/64th of points */ 137, /* horizontal device resolution */ 135 ); /* vertical device resolution */
	if (error) errorMessage(__LINE__,error);
	drawCstring("Casio Prizm",0,80);
	drawCstring("Freetype2",0,192);
	FT_Done_FreeType(ft);
	while(1) GetKey(&key);
}
