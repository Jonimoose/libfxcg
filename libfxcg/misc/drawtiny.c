#include <fxcg/display.h>
#include <font6x8ext.h>
void drawTinyC(char c,int x,int y,int fg,int bg){
	unsigned short * p=(unsigned short*)0xA8000000;
	unsigned char * d=font6x8ext+(((unsigned char)c)*8);
	p+=y*LCD_WIDTH_PX+x;
	int b;
	if((fg>=0)&&(bg>=0)){
		for(b=0;b<8;++b){
			int a;
			for(a=8-1;a>=2;--a){
				if((*d>>a)&1)
					*p++=fg;
				else
					*p++=bg;
			}
			p+=LCD_WIDTH_PX-6;
			++d;
		}
	}else if(fg>=0){
		for(b=0;b<8;++b){
			int a;
			for(a=8-1;a>=2;--a){
				if((*d>>a)&1)
					*p=fg;
				++p;
			}
			p+=LCD_WIDTH_PX-6;
			++d;
		}
	}else if (bg>=0){
		for(b=0;b<8;++b){
			int a;
			for(a=8-1;a>=2;--a){
				if(!((*d>>a)&1))
					*p=bg;
				++p;
			}
			p+=LCD_WIDTH_PX-6;
			++d;
		}
	}
}
static void clrBg(int bg){
	memmove((unsigned short*)0xA8000000+(24*LCD_WIDTH_PX),(unsigned short*)0xA8000000+(40*LCD_WIDTH_PX),2*(LCD_HEIGHT_PX-40)*LCD_WIDTH_PX);//Move everything up two "lines" (16 pixels)
	if(bg>=0){
		unsigned int c32=bg|(bg<<16);
		unsigned int * V=(unsigned int*)0xA8000000;
		V+=(LCD_HEIGHT_PX-16)*LCD_WIDTH_PX/2;
		int l=16*LCD_WIDTH_PX/2;
		while(l--)
			*V++=c32;
	}
}
static void checkDim(int *x,int *y,int *yd,int bg){
	if(*x>(LCD_WIDTH_PX-6)){
		*x=0;
		if(yd[0]<0)
			yd[0]=*y;
		*y+=8;
		if(*y>=LCD_HEIGHT_PX){
			yd[0]=24;
			clrBg(bg);
			*y-=16;
		}
		yd[1]=*y+8;
	}
}
static void insideLoop(char * s,int *x,int *y,int fg,int bg,int *yd){
	int a=*x,b=*y;//try keeping these in cpu registers
	if(*s=='\n'){
		a=0;
		if(yd[0]<0)
			yd[0]=b;
		b+=8;
		if(b>=LCD_HEIGHT_PX){
			yd[0]=24;
			clrBg(bg);
			b-=16;
		}
		yd[1]=b+8;
	}else if(*s=='\t'){
		if(a>=(LCD_WIDTH_PX-30)){
			a=0;
			if(yd[0]<0)
				yd[0]=b;
			b+=8;
			if(b>=LCD_HEIGHT_PX){
				yd[0]=24;
				clrBg(bg);
				b-=16;
			}
			yd[1]=b+8;
		}else{
			if(!(a%24)){
				drawTinyC(' ',a,b,fg,bg);
				a+=6;
			}
			while(a%24){
				drawTinyC(' ',a,b,fg,bg);
				a+=6;
			}
		}
	}else if(*s=='\r')
		a=0;
	else{
		drawTinyC(*s,a,b,fg,bg);
		a+=6;
		*x=a;
		*y=b;
		checkDim(x,y,yd,bg);
		a=*x;
		b=*y;
	}
	*x=a;
	*y=b;
}
void drawTinyStr(const char * s,int *x,int *y,int fg,int bg){
	//Draws a string on screen handles word-wrapping and newlines
	//Use -1 on fg or bg to not put pixels with that color
	int yd[2];
	yd[0]=-1;
	checkDim(x,y,yd,bg);
	while(*s){//Properly handle the unlikely case of the first character being null terminator
		insideLoop(s,x,y,fg,bg,yd);
		++s;
	}
	if(yd[0]>0)
		Bdisp_PutDisp_DD_stripe(yd[0],yd[1]);
}
void drawTinyStrn(const char * s,int *x,int *y,int fg,int bg,int n){
	int yd[2];
	yd[0]=-1;
	checkDim(x,y,yd,bg);
	while(n--){//This function does not care about null terimnators
		insideLoop(s,x,y,fg,bg,yd);
		++s;
	}
	if(yd[0]>0)
		Bdisp_PutDisp_DD_stripe(yd[0],yd[1]);
}
