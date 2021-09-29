/* Based on http://freespace.virgin.net/hugo.elias/graphics/x_stars.htm */
#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/rtc.h>
#define STARSAMT 64
static void DrawWuLine(int X0, int Y0, int X1, int Y1, int BaseColor, int NumLevels, unsigned int IntensityBits);
static void DrawWuLineClear(int X0, int Y0, int X1, int Y1);
static unsigned newStar(int*x,int*y,int*z,int*zv,unsigned seed){
    *x=seed%2516582400-1258291200;
    seed=seed*1103515245+12345;
    *y=seed%1415577600-707788800;
    seed=seed*1103515245+12345;
    *z=seed%29491200+3276800;
    seed=seed*1103515245+12345;
    *zv=seed%655360;
    seed=seed*1103515245+12345;
    return seed;
}
static inline unsigned sqrti(unsigned a){
    /*From: http://www.embedded.com/electronics-blogs/programmer-s-toolbox/4219659/Integer-Square-Roots
     * The reason I used this version of code is because it is better for small numbers*/
    unsigned sq=1;
    unsigned d=3;
    while(sq<=a){
        sq+=d;
        d+=2;
    }
    return (d/2-1);
}
static int keyPressed(int basic_keycode){
    const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
    int row, col, word, bit;
    row = basic_keycode%10;
    col = basic_keycode/10-1;
    word = row>>1;
    bit = col + ((row&1)<<3);
    return (0 != (keyboard_register[word] & 1<<bit));
}
void main(void){
    int starx[STARSAMT],stary[STARSAMT],starz[STARSAMT],starzv[STARSAMT];//Fixed point location and scale of stars Q15 format
    int screenx[STARSAMT],screeny[STARSAMT],screenxold[STARSAMT],screenyold[STARSAMT];//This is kept track of to simulate motion blur
    Bdisp_EnableColor(1);//Enable 16-bit mode
    memset(screenx,0,64*sizeof(int));
    memset(screeny,0,64*sizeof(int));
    memset(screenxold,0,64*sizeof(int));
    memset(screenyold,0,64*sizeof(int));
    memset(GetVRAMAddress(),0,384*216*2);
    unsigned seed=RTC_GetTicks(),i;
    for(i=0;i<STARSAMT;++i)
        seed=newStar(starx+i,stary+i,starz+i,starzv+i,seed);
    for(;;){
        for(i=0;i<STARSAMT;++i){
            /* Clear old star */
            int x,y;
            starz[i]-=starzv[i];
            x=starx[i]/starz[i]+160;
            y=stary[i]/starz[i]+108;
            if((x<=1)||(y<=1)||(x>=382)||(y>=214)||(starz[i]<32768)||(starz[i]>32768000)){
                seed=newStar(starx+i,stary+i,starz+i,starzv+i,seed);
                DrawWuLineClear(screenx[i],screeny[i],screenxold[i],screenyold[i]);
                screenx[i]=screeny[i]=screenxold[i]=screenyold[i]=0;
            }else{
                int xd,yd,length,b;
                xd=x-screenx[i];
                yd=y-screeny[i];
                length=sqrti((xd*xd)+(yd*yd));
                b=(6200*starzv[i])/starz[i];
                if(length>16) b/=length/8;
                if(screenxold[i]||screenyold[i]){
                    DrawWuLineClear(screenx[i],screeny[i],screenxold[i],screenyold[i]);
                    DrawWuLine(x,y,screenx[i],screeny[i],b&31,31,5);
                }
                screenxold[i]=screenx[i];
                screenyold[i]=screeny[i];
                screenx[i]=x;
                screeny[i]=y;
            }
        }
        if(keyPressed(KEY_PRGM_MENU)){
            int key;
            GetKey(&key);
        }
        Bdisp_PutDisp_DD();
    }
}
static void DrawPixelGray(int x,int y,int col){
    unsigned short*v= GetVRAMAddress();
    v+=y*384+x;
    *v=col*2113;//Convert from gray 0-31 to rgb565
}
static void DrawPixelSet(int x,int y,unsigned short col){
    unsigned short*v= GetVRAMAddress();
    v+=y*384+x;
    *v=col;
}
/* DrawWuLine from https://github.com/jagregory/abrash-black-book/blob/master/src/chapter-42.md */
static void DrawWuLine(int X0, int Y0, int X1, int Y1, int BaseColor, int NumLevels, unsigned int IntensityBits){
    unsigned int IntensityShift, ErrorAdj, ErrorAcc;
    unsigned int ErrorAccTemp, Weighting, WeightingComplementMask;
    int DeltaX, DeltaY, Temp, XDir;

    /* Make sure the line runs top to bottom */
    if (Y0 > Y1) {
        Temp = Y0; Y0 = Y1; Y1 = Temp;
        Temp = X0; X0 = X1; X1 = Temp;
    }
    /* Draw the initial pixel, which is always exactly intersected by
       the line and so needs no weighting */
    DrawPixelGray(X0, Y0, BaseColor);

    if ((DeltaX = X1 - X0) >= 0) {
        XDir = 1;
    } else {
        XDir = -1;
        DeltaX = -DeltaX; /* make DeltaX positive */
    }
    /* Special-case horizontal, vertical, and diagonal lines, which
       require no weighting because they go right through the center of
       every pixel */
    if ((DeltaY = Y1 - Y0) == 0) {
        /* Horizontal line */
        while (DeltaX-- != 0) {
            X0 += XDir;
            DrawPixelGray(X0, Y0, BaseColor);
        }
        return;
    }
    if (DeltaX == 0) {
        /* Vertical line */
        do {
            Y0++;
            DrawPixelGray(X0, Y0, BaseColor);
        } while (--DeltaY != 0);
        return;
    }
    if (DeltaX == DeltaY) {
        /* Diagonal line */
        do {
            X0 += XDir;
            Y0++;
            DrawPixelGray(X0, Y0, BaseColor);
        } while (--DeltaY != 0);
        return;
    }
    /* line is not horizontal, diagonal, or vertical */
    ErrorAcc = 0;  /* initialize the line error accumulator to 0 */
    /* # of bits by which to shift ErrorAcc to get intensity level */
    IntensityShift = 16 - IntensityBits;
    /* Mask used to flip all bits in an intensity weighting, producing the
       result (1 - intensity weighting) */
    WeightingComplementMask = NumLevels - 1;
    /* Is this an X-major or Y-major line? */
    if (DeltaY > DeltaX) {
        /* Y-major line; calculate 16-bit fixed-point fractional part of a
           pixel that X advances each time Y advances 1 pixel, truncating the
           result so that we won't overrun the endpoint along the X axis */
        ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
        /* Draw all pixels other than the first and last */
        while (--DeltaY) {
            ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
            ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
            if (ErrorAcc <= ErrorAccTemp) {
                /* The error accumulator turned over, so advance the X coord */
                X0 += XDir;
            }
            Y0++; /* Y-major, so always advance Y */
            /* The IntensityBits most significant bits of ErrorAcc give us the
               intensity weighting for this pixel, and the complement of the
               weighting for the paired pixel */
            Weighting = ErrorAcc >> IntensityShift;
            DrawPixelGray(X0, Y0, BaseColor + Weighting);
            DrawPixelGray(X0 + XDir, Y0,
                    BaseColor + (Weighting ^ WeightingComplementMask));
        }
        /* Draw the final pixel, which is always exactly intersected by the line
           and so needs no weighting */
        DrawPixelGray(X1, Y1, BaseColor);
        return;
    }
    /* It's an X-major line; calculate 16-bit fixed-point fractional part of a
       pixel that Y advances each time X advances 1 pixel, truncating the
       result to avoid overrunning the endpoint along the X axis */
    ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
    /* Draw all pixels other than the first and last */
    while (--DeltaX) {
        ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
        ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
        if (ErrorAcc <= ErrorAccTemp) {
            /* The error accumulator turned over, so advance the Y coord */
            Y0++;
        }
        X0 += XDir; /* X-major, so always advance X */
        /* The IntensityBits most significant bits of ErrorAcc give us the
           intensity weighting for this pixel, and the complement of the
           weighting for the paired pixel */
        Weighting = ErrorAcc >> IntensityShift;
        DrawPixelGray(X0, Y0, BaseColor + Weighting);
        DrawPixelGray(X0, Y0 + 1,
                BaseColor + (Weighting ^ WeightingComplementMask));
    }
    /* Draw the final pixel, which is always exactly intersected by the line
       and so needs no weighting */
    DrawPixelGray(X1, Y1, BaseColor);
}

static void DrawWuLineClear(int X0, int Y0, int X1, int Y1){
    unsigned int ErrorAdj, ErrorAcc;
    unsigned int ErrorAccTemp;
    int DeltaX, DeltaY, Temp, XDir;
    if (Y0 > Y1) {
        Temp = Y0; Y0 = Y1; Y1 = Temp;
        Temp = X0; X0 = X1; X1 = Temp;
    }
    DrawPixelSet(X0, Y0,0);

    if ((DeltaX = X1 - X0) >= 0) {
        XDir = 1;
    } else {
        XDir = -1;
        DeltaX = -DeltaX; /* make DeltaX positive */
    }
    if ((DeltaY = Y1 - Y0) == 0) {
        /* Horizontal line */
        while (DeltaX-- != 0) {
            X0 += XDir;
            DrawPixelSet(X0, Y0, 0);
        }
        return;
    }
    if (DeltaX == 0) {
        /* Vertical line */
        do {
            Y0++;
            DrawPixelSet(X0, Y0, 0);
        } while (--DeltaY != 0);
        return;
    }
    if (DeltaX == DeltaY) {
        /* Diagonal line */
        do {
            X0 += XDir;
            Y0++;
            DrawPixelSet(X0, Y0, 0);
        } while (--DeltaY != 0);
        return;
    }
    /* line is not horizontal, diagonal, or vertical */
    ErrorAcc = 0;  /* initialize the line error accumulator to 0 */
    if (DeltaY > DeltaX) {
        ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
        /* Draw all pixels other than the first and last */
        while (--DeltaY) {
            ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
            ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
            if (ErrorAcc <= ErrorAccTemp) {
                /* The error accumulator turned over, so advance the X coord */
                X0 += XDir;
            }
            Y0++; /* Y-major, so always advance Y */
            DrawPixelSet(X0, Y0,0);
            DrawPixelSet(X0 + XDir, Y0,0);
        }
        /* Draw the final pixel, which is always exactly intersected by the line
           and so needs no weighting */
        DrawPixelSet(X1, Y1,0);
        return;
    }
    ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
    /* Draw all pixels other than the first and last */
    while (--DeltaX) {
        ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
        ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
        if (ErrorAcc <= ErrorAccTemp) {
            /* The error accumulator turned over, so advance the Y coord */
            Y0++;
        }
        X0 += XDir; /* X-major, so always advance X */
        DrawPixelSet(X0, Y0,0);
        DrawPixelSet(X0, Y0 + 1,0);
    }
    DrawPixelSet(X1, Y1,0);
}

