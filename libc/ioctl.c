#include <sys/ioctl.h>
#include <fxcg/fddefs.h>
#include <linux/fb.h>
#include <stdio.h>
#include <sys/ttycom.h>
int ioctl(int d, unsigned long request, ...){
	switch(request){
		case TIOCGWINSZ:
			{va_list arguments;
			va_start(arguments,1);
			struct winsize * ws=va_arg(arguments,struct fb_fix_screeninfo *);
			va_end(arguments);
			ws->ws_row=64;
			ws->ws_col=24;
			ws->ws_xpixel=384;
			ws->ws_ypixel=192;//The first 24 pixels get covered by GetKey top banner
			}
		break;
		case FBIOGET_FSCREENINFO:
			if(d==frameBuf_FD){
				va_list arguments;
				va_start(arguments,1);
				struct fb_fix_screeninfo * i=va_arg(arguments,struct fb_fix_screeninfo *);
				va_end(arguments);
				i->line_length=384*2;
			}//It is in bytes not pixels
		break;
		case FBIOGET_VSCREENINFO:
			if(d==frameBuf_FD){
				va_list arguments;
				va_start(arguments,1);
				struct fb_var_screeninfo * i=va_arg(arguments,struct fb_var_screeninfo *);
				va_end(arguments);
				i->xres=384;
				i->yres=216;
				i->bits_per_pixel=16;
				i->xoffset=0;
			}
		default:
			fprintf(stderr,"ioctl is only emulation %d %d\n",d,request);
			return -1;
		break;
	}
	return 0;
}
