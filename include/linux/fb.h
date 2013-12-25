struct fb_var_screeninfo{
	int xres;
	int yres;
	int bits_per_pixel;
	int xoffset;
	int yoffset;
};
struct fb_fix_screeninfo{
	int line_length;
};
