#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <font6x8ext.h>
#include <stdio.h>
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
	memmove((unsigned short*)0xA8000000+(24*LCD_WIDTH_PX),(unsigned short*)0xA8000000+(32*LCD_WIDTH_PX),2*(LCD_HEIGHT_PX-32)*LCD_WIDTH_PX);//Move everything up one "line" (8 pixels)
	if(bg>=0){
		unsigned int c32=bg|(bg<<16);
		unsigned int * V=(unsigned int*)0xA8000000;
		V+=(LCD_HEIGHT_PX-8)*LCD_WIDTH_PX/2;
		int l=8*LCD_WIDTH_PX/2;
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
			*y-=8;
		}
		yd[1]=*y+8;
	}
}
static void drawIt(char * s,int *x,int *y,int fg,int bg,int *yd){
	drawTinyC(*s,*x,*y,fg,bg);
	*x+=6;
	checkDim(x,y,yd,bg);
}
static void insideLoop(char * s,int *x,int *y,int fg,int bg,int *yd){
	if(*s=='\n'){
		*x=0;
		if(yd[0]<0)
			yd[0]=*y;
		*y+=8;
		if(*y>=LCD_HEIGHT_PX){
			yd[0]=24;
			clrBg(bg);
			*y-=8;
		}
		yd[1]=*y+8;
	}else if(*s=='\t'){
		if(*x>=(LCD_WIDTH_PX-30)){
			*x=0;
			if(yd[0]<0)
				yd[0]=*y;
			*y+=8;
			if(*y>=LCD_HEIGHT_PX){
				yd[0]=24;
				clrBg(bg);
				*y-=8;
			}
			yd[1]=*y+8;
		}else{
			if(!(*x%24)){
				drawTinyC(' ',*x,*y,fg,bg);
				*x+=6;
			}
			while(*x%24){
				drawTinyC(' ',*x,*y,fg,bg);
				*x+=6;
			}
		}
	}else if(*s=='\r'){
		*x=0;
	}else if (*s==0x1B){
		if(*(++s)=='['){
			drawIt(--s,x,y,0x001F,bg,yd);
		}else
			drawIt(--s,x,y,fg,bg,yd);
	}else{
		drawIt(s,x,y,fg,bg,yd);
	}
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
void drawTinyStrnn(const char * s,int *x,int *y,int fg,int bg,int n){
	int yd[2];
	yd[0]=-1;
	checkDim(x,y,yd,bg);
	while(n--){
		insideLoop(s,x,y,fg,bg,yd);
		++s;
		if(!(*s))
			break;
	}
	if(yd[0]>0)
		Bdisp_PutDisp_DD_stripe(yd[0],yd[1]);
}
size_t inputStrTiny(unsigned char * str,size_t max,int newline){
	memset(str,0,max);
	int key;
	int termxold=termx;
	int termyold=termy;
	int offset=0,pos=0,lower=0;
	while(1){
		GetKey(&key);
		if(key==KEY_CTRL_EXE){
			//See if possible to add newline
			if((max>1)&&(newline!=0))
				strcat(str,"\n");
			break;
		}else if(key==KEY_CTRL_EXIT){
			break;
		}else if(key==KEY_CTRL_LEFT){
			if(pos)
				--pos;
		}else if(key==KEY_CTRL_RIGHT){
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CTRL_F1){
			lower^=1;
		}else if((key>=KEY_CHAR_0)&&(key<=KEY_CHAR_9)){
			str[pos]=key-KEY_CHAR_0+'0';
			if(pos<(max-1)) ++pos;
		}else if((key>=KEY_CHAR_A)&&(key<=KEY_CHAR_Z)){
			if(lower)
				str[pos]=key-KEY_CHAR_A+'a';
			else
				str[pos]=key-KEY_CHAR_A+'A';
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CHAR_COMMA){
			str[pos]=',';
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CHAR_SPACE){
			str[pos]=' ';
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CHAR_EQUAL){
			str[pos]='=';
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CHAR_PLUS){
			str[pos]='+';
			if(pos<(max-1)) ++pos;
		}else if ((key==KEY_CHAR_MINUS)||(key==KEY_CHAR_PMINUS)){
			str[pos]='-';
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CHAR_MULT){
			str[pos]='*';
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CHAR_DIV){
			str[pos]='/';
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CHAR_LBRCKT){
			str[pos]='[';
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CHAR_RBRCKT){
			str[pos]=']';
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CHAR_LPAR){
			str[pos]='(';
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CHAR_RPAR){
			str[pos]=')';
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CHAR_POW){
			str[pos]='^';
			if(pos<(max-1)) ++pos;
		}else if (key==KEY_CTRL_DEL){
			if(pos==0){
				if(max>1){
					memmove(str,str+1,max-1);
				}else{
					str[pos]=0;
				}
			}else if(pos<(max-1)){
				memmove(str+pos,str+pos+1,max-pos-1);
				drawTinyC(' ',termx-6,termy,0,0xFFFF);
			}
		}
		termx=termxold;
		termy=termyold;
		drawTinyStrnn(str,&termx,&termy,0xFFFF,0,max);
		drawTinyC(str[pos],termxold+((pos&63)*6),termyold+((pos/64)*8),0,0xFFFF);//draw cursor			
	}
	putchar('\n');
	return strlen(str);
}
