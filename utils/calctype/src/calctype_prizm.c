#include "calctype.h"
#include "fxcg\display.h"

unsigned char* CalcType_DefaultVRAM() {
	// direct to screen
	return (unsigned char*) GetVRAMAddress();
}

unsigned int CalcType_DefaultPitch() {
	// screen width
	return 384;
}

#define RED_PART(x) ((x & 0xF800) >> 11)
#define GREEN_PART(x) ((x & 0x07E0) >> 5)
#define BLUE_PART(x) (x & 0x001F)

void CalcType_DrawGlyph(CalcTypeCharData* glyph, int x, int y, unsigned short color, unsigned char* vram, unsigned int pitch) {
	unsigned short* drawTo = ((unsigned short*)vram) + x + y * pitch;
	unsigned char* data = glyph->data;
	int drawIncrement = pitch - glyph->width;

	// in order for my fast interpolation to work, the inverse of the mask value
	// must be the bit inverse as well to prevent color overlap
	static const unsigned short redMask[8] =
	{
		0b0000000000000000,
		0b0011100000000000,
		0b0101100000000000,
		0b0111100000000000,
		0b1000000000000000,
		0b1010000000000000,
		0b1100000000000000,
		0b1111100000000000,
	};

	static const unsigned short greenMask[8] =
	{
		0b0000000000000000,
		0b0000000111100000,
		0b0000001011000000,
		0b0000001111100000,
		0b0000010000000000,
		0b0000010100100000,
		0b0000011000000000,
		0b0000011111100000,
	};

	static const unsigned short blueMask[4] =
	{
		0b0000000000000000,
		0b0000000000001111,
		0b0000000000010000,
		0b0000000000011111,
	};

	for (int gy = 0; gy < glyph->height; gy++) {
		for (int gx = 0; gx < glyph->width; gx++, drawTo++, data++) {
			// glyph data is packed 3:4:1
			int screen = *drawTo;
			int fontData = data[0];
			int invFontData = ~fontData;

			*drawTo =
				// mask in draw color
				(color & redMask[(fontData & 0xE0) >> 5]) |
				(color & greenMask[(fontData & 0x1C) >> 2]) |
				(color & blueMask[fontData & 0x03]) |
				// mask out screen color
				(screen & redMask[(invFontData & 0xE0) >> 5]) |
				(screen & greenMask[(invFontData & 0x1C) >> 2]) |
				(screen & blueMask[invFontData & 0x03]);
		}
		drawTo += drawIncrement;
	}
}