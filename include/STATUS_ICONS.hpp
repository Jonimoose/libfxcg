#ifndef __STATUS_ICONS_HPP__
#define __STATUS_ICONS_HPP__

#define STICON_WIDTH 0x0030
#define STICON_HEIGHT 0x0018

enum TSticon { sticEMPTY, sticHEX, sticASCII, sticADDR, sticLEN };

typedef struct{
    short    dx;
    short    dy;
    void*    shortcolor_bitmap;
    short    xalignment;    // 1, 2 or 3
    short    xoffset;    // usually 1
} TStatusGlyphDefinition;


void*GetStatusIconPtr( TSticon stic );
void SetStatusIcon( TSticon stic );

#endif