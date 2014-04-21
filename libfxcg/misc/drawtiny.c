#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <font6x8ext.h>
#include <stdio.h>
#include <string.h>
#define DEBUGTERM 1
//#define VERBOSEDEBUG 1
void drawTinyC(char c,int x,int y,int fg,int bg){
	unsigned short * p=(unsigned short*)0xA8000000;
	unsigned char * d=(unsigned char *)font6x8ext+(((unsigned char)c)*8);
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
static void fillAreaWithColor(int y1,int y2,int bg){
	if(bg>=0){
		int a=(y2-y1)*LCD_WIDTH_PX/2,c=bg|(bg<<16),*v=(int *)0xA8000000+(y1*LCD_WIDTH_PX/2);
			while(a--)
				*v++=c;
	}
}
static void clearScr(int bg){
	fillAreaWithColor(0,216,bg);
	Bdisp_PutDisp_DD();
}
static void scrollUP(int l,int bg,int *yd){
	//Move stuff down
	if(l>24){
		clearScr(bg);
		yd[0]=-1;
	}else{
		yd[0]=24;
		yd[1]=216;
		memmove((unsigned short *)0xA8000000+((24+(l*8))*LCD_WIDTH_PX),(unsigned short *)0xA8000000+(24*LCD_WIDTH_PX),(216-24-(l*8))*LCD_WIDTH_PX*2);
		fillAreaWithColor(24,24+(l*8),bg);
	}
}
static int savedX,savedY;
static int savedBG,savedFG;
static int isNum(char c){
	return ((c>='0')&&(c<='9'));
}
static int isSep(char c){
	return ((c==';')||(c==':'));
}
static int cntParm(char * s){
	int amt=0;
	if(isNum(*s)){
		++amt;
		while(1){
			while(isNum(*s++));
			--s;
			if(isSep(*s++))
				++amt;
			else
				break;
		}
	}
	return amt;
}
static char * getParm(char * s,int * p,int amt){
	int i=0;
	while(amt--){
		p[i++]=strtol(s,&s,10);
		++s;
	}
	return --s;
}
#define rgb888to565(r,g,b) (((r&248)<<8)|((g&252)<<3)|((b&248)>>3))
static void setColxtermext(int * c,int * p){
	if(p[1]==2){
		*c=rgb888to565(p[2],p[3],p[4]);
	}else if(p[1]==5){
		
	}
}
static void cursorBack(int *x,int *y,int *yd,int bg){
	if(*x>=6){
		*x-=6;
	}else{
		if(*y<24)
			scrollUP(1,bg,yd);
		else
			*y-=8;
		*x=63*6;
	}
}
#ifdef DEBUGTERM
static void clrDebugLn(void){
	memset((unsigned short *)0xA8000000+(24*384),0,10*384*2);
}
static void notSupportedCSI(char s,int *p,int amt,int leadingM){
	char *tmp=alloca(amt*16+32);
	sprintf(tmp,"%d %d %d %c ",leadingM,amt,s,s);
	int i;
	for(i=0;i<amt;++i)
		sprintf(tmp+strlen(tmp),"p[%d]=%d ",i,p[i]);
	int xx=0,yy=0;
	PrintMiniMini(&xx, &yy, tmp, 0, 0, 0);
	GetKey(&xx);
	clrDebugLn();
}
static void cursorExceed(char w,int x){
	char tmp[128];
	sprintf(tmp,"cursor %c exceeded %d",w,x);
	int xx=0,yy=0;
	PrintMiniMini(&xx, &yy, tmp, 0, 0, 0);
	GetKey(&xx);
	clrDebugLn();
}
#endif
static void eraseRight(int x,int y,int bg){
	if(bg>=0){
		int a=8,c=bg|(bg<<16),*v=(int *)0xA8000000+((y+24)*384/2);
		while(a--){
			int b=x;
			while(b--)
				*v++=c;
			v+=(384-x)/2;
		}
	}
}
static char * handleCSI(char * s,int *x,int *y,int *fg,int *bg,int *yd){
	//s points to character after CSI character(s)
	#ifdef VERBOSEDEBUG
		{int xx=0,yy=0;
		PrintMiniMini(&xx, &yy, s, 0, 0, 0);
		GetKey(&xx);
		clrDebugLn();}
	#endif
	int leadingM;
	if(*s=='?'){
		leadingM=1;
		++s;
	}else
		leadingM=0;
	int amt=cntParm(s);
	int * p;
	int al;
	if(amt)
		al=amt;
	else
		al=1;
	p=alloca(al*sizeof(int));//It would be more logical to put the control character first then the numbers but I did not design this system
	if(amt)
		s=getParm(s,p,amt);
	else
		p[0]=1;
	switch(leadingM){
		case 0:
			switch(*s){//At this point *s should contain the "control character"
				case '@':
					while(p[0]--){
						drawTinyC(' ',*x,*y,*fg,*bg);
						*x+=6;
						checkDim(x,y,yd,*bg);
					}
				break;
				case 'A':
					scrollUP(p[0],*bg,yd);
					if(yd[0]>=0)
						Bdisp_PutDisp_DD_stripe(yd[0],yd[1]);
					yd[0]=-1;
				break;
				case 'D':
					while(p[0]--){
						cursorBack(x,y,yd,*bg);
					}
				break;
				case 'G'://Cursor position Absolute column
					*y=p[0]*8+24;
				break;
				case 'H':
					switch(amt){
						case 0:
							*x=6;
							*y=8+24;
						break;
						case 1:
							*x=p[0]*6;
							if((*x>378)||(*x<0)){
								#ifdef DEBUGTERM
									cursorExceed('x',*x);
								#endif
								*x=378;
							}
							*y=8+24;
						break;
						case 2:
							*x=p[0]*6;
							if((*x>378)||(*x<0)){
								#ifdef DEBUGTERM
									cursorExceed('x',*x);
								#endif
								*x=378;
							}
							*y=p[1]*8+24;
							if(*y>=208){
								#ifdef DEBUGTERM
									cursorExceed('y',*y);
								#endif
								*y=208;
							}
						break;
					}
				break;
				case 'J'://Erase in display
					if(amt){
						switch(p[0]){
							case 0://Below (default)
								fillAreaWithColor(*y,216,*bg);
							break;
							case 1://Above
								fillAreaWithColor(0,*y,*bg);
							break;
							case 2://All
								fillAreaWithColor(0,216,*bg);
							break;
						}
					}else{
						fillAreaWithColor(*y,216,*bg);
					}
				break;
				case 'K'://Erase in lines
					if(amt){
						switch(p[0]){
							case 0://Right (default)
								//eraseRight(*x,*y,*bg);
							break;
							case 1://Left
								
							break;
							case 2://All
								fillAreaWithColor(*y,*y+8,*bg);
							break;
						}
					}else{
						//eraseRight(*x,*y,*bg);
					}
				break;
				case 'P'://erase characters
				
				break;
				case 'd'://Line position absolute
					
				break;
				case 'm':
					//set color
					if(amt>0){//TODO bold and underline
						switch(p[0]){
							case 0:
								//Reset to default
								*bg=0;
								*fg=0xFFFF;
							break;
							case 30://black
								if(*fg>=0)
									*fg=0;
							break;
							case 31://Red
								if(*fg>=0)
									*fg=rgb888to565(194,54,33);
							break;
							case 32://Green
								if(*fg>=0)
									*fg=rgb888to565(37,188,36);
							break;
							case 33://Yellow
								if(*fg>=0)
									*fg=rgb888to565(173, 173, 39);
							break;
							case 34://Blue
								if(*fg>=0)
									*fg=rgb888to565(73, 46, 225);
							break;
							case 35://Magenta
								if(*fg>=0)
									*fg=rgb888to565(211, 56, 211);
							break;
							case 36://Cyan
								if(*fg>=0)
									*fg=rgb888to565(51, 187, 200);
							break;
							case 37://White
								if(*fg>=0)
									*fg=0xFFFF;
							break;
							case 38://Extened
								setColxtermext(bg,p);
							break;
							case 39://Default
								if(*fg>=0)
									*fg=0xFFFF;
							break;
							case 40://black
								if(*bg>=0)
									*bg=0;
							break;
							case 41://Red
								if(*bg>=0)
									*bg=rgb888to565(194,54,33);
							break;
							case 42://Green
								if(*bg>=0)
									*bg=rgb888to565(37,188,36);
							break;
							case 43://Yellow
								if(*bg>=0)
									*bg=rgb888to565(173, 173, 39);
							break;
							case 44://Blue
								if(*bg>=0)
									*bg=rgb888to565(73, 46, 225);
							break;
							case 45://Magenta
								if(*bg>=0)
									*bg=rgb888to565(211, 56, 211);
							break;
							case 46://Cyan
								if(*bg>=0)
									*bg=rgb888to565(51, 187, 200);
							break;
							case 47://White
								if(*bg>=0)
									*bg=0xFFFF;
							break;
							case 48://extension
								setColxtermext(bg,p);
							break;
							case 49://default
								if(bg>0)
									*bg=0;
							break;
						}
					}else{//default
						*bg=0;
						*fg=0xFFFF;
					}
				break;
				case 's':
					savedX=*x;
					savedY=*y;
					savedBG=*bg;
					savedFG=*fg;
				break;
				case 'u':
					*x=savedX;
					*y=savedY;
					*bg=savedBG;
					*fg=savedFG;
				break;
				default:
					#ifdef DEBUGTERM
						notSupportedCSI(*s,p,amt,leadingM);
					#endif
				break;
			}
		break;
		case 1:
			switch(*s){
				case 'h':
					switch(p[0]){
						case 1048:
							savedX=*x;
							savedY=*y;
						break;
						case 1049:
							savedX=*x;
							savedY=*y;
							clearScr(*bg);
							yd[0]=-1;
						break;
					}
				break;
				default:
					#ifdef DEBUGTERM
						notSupportedCSI(*s,p,amt,leadingM);
					#endif
				break;
			}
		break;
		default:
			#ifdef DEBUGTERM
				notSupportedCSI(*s,p,amt,leadingM);
			#endif
		break;
	}
	return s;
}
static char * insideLoop(unsigned char * s,int *x,int *y,int *fg,int *bg,int *yd){
	switch(*s){
		case '\v':
		case '\n':
			*x=0;
			if(yd[0]<0)
				yd[0]=*y;
			*y+=8;
			if(*y>=LCD_HEIGHT_PX){
				yd[0]=24;
				clrBg(*bg);
				*y-=8;
			}
			yd[1]=*y+8;
		break;
		case '\t':
			if(*x>=(LCD_WIDTH_PX-30)){
				*x=0;
				if(yd[0]<0)
					yd[0]=*y;
				*y+=8;
				if(*y>=LCD_HEIGHT_PX){
					yd[0]=24;
					clrBg(*bg);
					*y-=8;
				}
				yd[1]=*y+8;
			}else{
				if(!(*x%24)){
					drawTinyC(' ',*x,*y,*fg,*bg);
					*x+=6;
				}
				while(*x%24){
					drawTinyC(' ',*x,*y,*fg,*bg);
					*x+=6;
				}
			}
		break;
		case '\r':
			*x=0;
		break;
		case '\f':
			//clear screen
			*x=0;
			*y=24;
			clearScr(*bg);
			yd[0]=-1;
		break;
		case '\b':
			drawTinyC(' ',*x,*y,*fg,*bg);
			cursorBack(x,y,yd,*bg);
		break;
		case 0x1B://ESC
			switch(*(++s)){
				case '7':
					savedX=*x;
					savedY=*y;
				break;
				case '8':
					*x=savedX;
					*y=savedY;
				break;
				case '[':
					s=handleCSI(++s,x,y,fg,bg,yd);
				break;
				default:
					drawIt(--s,x,y,*fg,*bg,yd);
			}
		break;
		case 0x9B:
			s=handleCSI(++s,x,y,fg,bg,yd);
		break;
		default:
			drawIt(s,x,y,*fg,*bg,yd);
	}
	return ++s;
}
int holdStrTERM=0;
static char stdioBuffer[24];
static void drawHeld(char * s,int *x,int *y,int * fg,int * bg,int n){
	int yd[2];
	while(n>0){
		char * ss=insideLoop(s,x,y,fg,bg,yd);
		n-=ss-s;
		s=ss;
	}
}
static int isTermC(char c){
	return ((c=='?'))||(c=='[');
}
static char * holdStr(char * c,int *x,int *y,int *fg,int *bg){//Returns 0 if not held
	unsigned char cc = (unsigned char)*c;
	if(holdStrTERM){
		if(holdStrTERM==24){
			drawHeld(stdioBuffer,x,y,fg,bg,24);
			holdStrTERM=1;
			stdioBuffer[0]=cc;
		}else if(!(isNum(cc)||isSep(cc)||isTermC(cc))){
			stdioBuffer[holdStrTERM++]=cc;
			drawHeld(stdioBuffer,x,y,fg,bg,holdStrTERM);
			holdStrTERM=0;
		}else
			stdioBuffer[holdStrTERM++]=cc;
	}else if((cc==0x1B)||(cc==0x9B)){
		holdStrTERM=1;
		memset(stdioBuffer,0,24);
		stdioBuffer[0]=cc;
	}else
		return 0;
	return ++c;
}
void drawTinyStr(char * s,int *x,int *y,int * fg,int * bg){
	//Draws a string on screen handles word-wrapping and newlines
	//Use -1 on fg or bg to not put pixels with that color
	int yd[2];
	yd[0]=-1;
	checkDim(x,y,yd,*bg);
	while(*s){//Properly handle the unlikely case of the first character being null terminator
		char * hs=holdStr(s,x,y,fg,bg);
		if(hs)
			s=hs;
		else
			s=insideLoop(s,x,y,fg,bg,yd);
	}
	if(yd[0]>=0)
		Bdisp_PutDisp_DD_stripe(yd[0],yd[1]);
}
void drawTinyStrn(char * s,int *x,int *y,int *fg,int *bg,int n){
	int yd[2];
	yd[0]=-1;
	checkDim(x,y,yd,*bg);
	while(n>0){//This function does not care about null terimnators
		char *hs=holdStr(s,x,y,fg,bg);
		if(hs){
			n-=hs-s;
			s=hs;
		}else{
			char * ss=insideLoop(s,x,y,fg,bg,yd);
			n-=ss-s;
			s=ss;
		}
	}
	if(yd[0]>=0)
		Bdisp_PutDisp_DD_stripe(yd[0],yd[1]);
}
void drawTinyStrnn(char * s,int *x,int *y,int *fg,int *bg,int n){
	int yd[2];
	yd[0]=-1;
	checkDim(x,y,yd,*bg);
	while(n>0){
		char *hs=holdStr(s,x,y,fg,bg);
		if(hs){
			n-=hs-s;
			s=hs;
		}else{
			char * ss=insideLoop(s,x,y,fg,bg,yd);
			n-=ss-s;
			s=ss;
			if(!(*s))
				break;
		}
	}
	if(yd[0]>=0)
		Bdisp_PutDisp_DD_stripe(yd[0],yd[1]);
}
size_t inputStrTiny(unsigned char * str,size_t max,int newline){
	memset(str,0,max);
	int key;
	int termxold=termxfxcg;
	int termyold=termyfxcg;
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
				drawTinyC(' ',termxfxcg-6,termyfxcg,0,0xFFFF);
			}
		}
		termxfxcg=termxold;
		termyfxcg=termyold;
		int bg,fg;
		bg=0xFFFF;
		fg=0;
		drawTinyStrnn(str,&termxfxcg,&termyfxcg,&bg,&fg,max);
		drawTinyC(str[pos],termxold+((pos&63)*6),termyold+((pos/64)*8),0,0xFFFF);//draw cursor			
	}
	putchar('\n');
	return strlen((const char *)str);
}
