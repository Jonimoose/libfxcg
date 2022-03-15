/*
	CalcType is a quasi-sub pixel rendered font library
 */

#pragma once

// the main font struct
typedef struct __CalcTypeFont {
	unsigned int height;				// pixel height of the font line in pixels
	unsigned int base;					// y offset of base font line
	unsigned int space;					// width of the font space in sub-pixels
	const unsigned char* charData;		// actual character data
	unsigned short charOffset[224];		// offset to each char's data (minus 32), 0xFFFF if not available
} CalcTypeFont;

// faux char data struct (it's embedded directly in byte data by CalcTyper
typedef struct _CalcTypeCharData {
	signed char xOffset;			// x offset (in subpixels) of the glyph
	signed char yOffset;			// y offset (in real pixels) of the glyph
	unsigned char xAdvance;			// amount (in subpixels) to advance x value
	unsigned char width;			// width (in real pixels) of the data, though its subpixel encodd
	unsigned char height;			// height (in real pixels) of the data
	unsigned char data[1];			// pointer helper for data 
									// data is encoded in RGB bytes for each pixel 3:4:1 (based on usefulness for intensity) 
} CalcTypeCharData;

#ifdef __cplusplus
extern "C" {
#endif

/*
	Returns the width in pixels of the given text using the given font
 */
unsigned int CalcType_Width(const CalcTypeFont* font, const char* text);

/*
	Draws the given font data to the given position. Use 0 for vram and pitch to use device defaults.

	color is "platform" encoded color
 */
void CalcType_Draw(const CalcTypeFont* font, const char* text, int x, int y, unsigned short color, unsigned char* vram, unsigned int pitch);

#ifdef __cplusplus
};
#endif