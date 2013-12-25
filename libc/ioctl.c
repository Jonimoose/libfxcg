#include <sys/ioctl.h>
#include <fxcg/fddefs.h>
#include <linux/fb.h>
#include <stdio.h>
int ioctl(int d, unsigned long request, ...){
	if(d==frameBuf_FD){
		if(request==FBIOGET_FSCREENINFO){
			va_list arguments;
			va_start(arguments,1);
			struct fb_fix_screeninfo * i=va_arg(arguments,struct fb_fix_screeninfo * );
			va_end(arguments);
			i->line_length=384*2;//It is in bytes not pixels
			
		}else if(request==FBIOGET_VSCREENINFO){
			va_list arguments;
			va_start(arguments,1);
			struct fb_var_screeninfo * i=va_arg(arguments,struct fb_var_screeninfo * );
			va_end(arguments);
			i->xres=384;
			i->yres=216;
			i->bits_per_pixel=16;
			i->xoffset=0;
			i->yoffset=0;
		}else
			fprintf(stderr,"Unknown ioctl framebuffer request %d\n",request);
		return 0;
	}else{
		fprintf(stderr,"ioctl is only emulation %d %d\n",d,request);
		return -1;
	}
}
