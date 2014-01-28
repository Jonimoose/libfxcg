// Draw a graph
#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/keyboard.h>
#include <fxcg/system.h>
#include <fxcg/misc.h>
#include <fxcg/app.h>
#include <fxcg/serial.h>
#include <fxcg/rtc.h>
#include <fxcg/heap.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphicsProvider.hpp"

#include "stdafx.h"
#include "defs.h"

#define DIMX 350
#define DIMY 190

#define F p3
#define T p4
#define X p5
#define Y p6
#define XT p7
#define YT p8

static double tmin, tmax;
static double xmin, xmax;
static double ymin, ymax;

#define YMAX 10000

static struct {
	int x, y;
	double t;
} draw_buf[YMAX];

static int draw_count;

void
eval_draw(void)
{
	F = cadr(p1);
	T = caddr(p1);

	if (T == symbol(NIL)) {
		push(F);
		rewrite();
		guess();
		T = pop();
		F = pop();
	}

	push(get_binding(T));
	push(get_arglist(T));

	draw_main();

	p2 = pop();
	p1 = pop();
	set_binding_and_arglist(T, p1, p2);

	// return value

	push(symbol(NIL));
}

void
draw_main(void)
{
	if (draw_flag) {
		draw_flag = 0; // so "stop" really stops
		stop("draw calls draw");
	}

	draw_flag++;

	setup_trange();

	setup_xrange();

	setup_yrange();

	check_for_parametric_draw();

	create_point_set();

	emit_graph();

	draw_flag--;
}

/*	xrange sets the horizontal scale

	yrange sets the vertical scale

	Normally, the function F is evaluated from xrange[1] to xrange[2].

	However, if F returns a vector then parametric drawing is used. In this
	case F is evaluated from trange[1] to trange[2].
*/

void
check_for_parametric_draw(void)
{
	eval_f(tmin);
	p1 = pop();
	if (!istensor(p1)) {
		tmin = xmin;
		tmax = xmax;
	}
}

#define N 100

void
create_point_set(void)
{
	int i, n;
	double t;

	draw_count = 0;

	for (i = 0; i <= N; i++) {
		t = tmin + ((double) i / (double) N) * (tmax - tmin);
		new_point(t);
	}

	n = draw_count;

	for (i = 0; i < n - 1; i++)
		fill(i, i + 1, 0);
}

void
new_point(double t)
{
	double x, y;

	if (draw_count >= YMAX)
		return;

	draw_buf[draw_count].x = -10000;
	draw_buf[draw_count].y = -10000;
	draw_buf[draw_count].t = t;

	draw_count++;

	get_xy(t);

	if (!isnum(XT) || !isnum(YT))
		return;

	push(XT);
	x = pop_double();
	x = (x - xmin) / (xmax - xmin);
	x = (double) DIMX * x + 0.5; // map 0-1 to 0-DIM, +0.5 so draw(x^3) looks right

	push(YT);
	y = pop_double();
	y = (y - ymin) / (ymax - ymin);
	y = (double) DIMY * y + 0.5; // map 0-1 to 0-DIM, +0.5 so draw(x^3) looks right

	if (x < -10000.0)
		x = -10000.0;
	if (x > 10000.0)
		x = 10000.0;
	if (y < -10000.0)
		y = -10000.0;
	if (y > 10000.0)
		y = 10000.0;

	draw_buf[draw_count - 1].x = (int) x;
	draw_buf[draw_count - 1].y = (int) y;
}

// Evaluate F(t) and return in XT and YT.

void
get_xy(double t)
{
	eval_f(t);

	p1 = pop();

	if (istensor(p1)) {
		if (p1->u.tensor->nelem >= 2) {
			XT = p1->u.tensor->elem[0];
			YT = p1->u.tensor->elem[1];
		} else {
			XT = symbol(NIL);
			YT = symbol(NIL);
		}
		return;
	}

	push_double(t);
	XT = pop();
	YT = p1;
}

// Evaluate F(t) without stopping due to an error such as divide by zero.

void
eval_f(double t)
{
	// These must be volatile or it crashes. (Compiler error?)
	// Read it backwards, save_tos is a volatile int, etc.

	int volatile save_tos;
	U ** volatile save_frame;

	save();
	save_tos = tos;
	save_frame = frame;

	draw_flag++;

	if (setjmp(draw_stop_return)) {
		tos = save_tos;
		push(symbol(NIL));
		frame = save_frame;
		restore();
		draw_flag--;
		return;
	}

	push_double(t);
	p1 = pop();
	set_binding(T, p1);

	push(F);
	eval();
	yyfloat();
	eval();

	restore();
	draw_flag--;
}

#define MAX_DEPTH 6

void
fill(int i, int k, int level)
{
	int dx, dy, j;
	double t;

	if (level >= MAX_DEPTH || draw_count >= YMAX)
		return;

	dx = abs(draw_buf[i].x - draw_buf[k].x);
	dy = abs(draw_buf[i].y - draw_buf[k].y);

	if (dx < 1 && dy < 1)
		return;

	t = (draw_buf[i].t + draw_buf[k].t) / 2.0;

	j = draw_count;

	new_point(t);

	fill(i, j, level + 1);
	fill(j, k, level + 1);
}

//-----------------------------------------------------------------------------
//
//	Normalize x to [0,1]
//
//	Example: xmin = -10, xmax = 10, xmax - xmin = 20
//
//	x		x - xmin	(x - xmin) / (xmax - xmin)
//
//	-10		0		0.00
//
//	-5		5		0.25
//
//	0		10		0.50
//
//	5		15		0.75
//
//	10		20		1.00
//
//-----------------------------------------------------------------------------

void
setup_trange(void)
{
	save();
	setup_trange_f();
	restore();
}

void
setup_trange_f(void)
{
	// default range is (-pi, pi)

	tmin = -M_PI;

	tmax = M_PI;

	p1 = usr_symbol("trange");

	if (!issymbol(p1))
		return;

	p1 = get_binding(p1);

	// must be two element vector

	if (!istensor(p1) || p1->u.tensor->ndim != 1 || p1->u.tensor->nelem != 2)
		return;

	push(p1->u.tensor->elem[0]);
	eval();
	yyfloat();
	eval();
	p2 = pop();

	push(p1->u.tensor->elem[1]);
	eval();
	yyfloat();
	eval();
	p3 = pop();

	if (!isnum(p2) || !isnum(p3))
		return;

	push(p2);
	tmin = pop_double();

	push(p3);
	tmax = pop_double();

	if (tmin == tmax)
		stop("draw: trange is zero");
}

void
setup_xrange(void)
{
	save();
	setup_xrange_f();
	restore();
}

void
setup_xrange_f(void)
{
	// default range is (-10,10)

	xmin = -10.0;

	xmax = 10.0;

	p1 = usr_symbol("xrange");

	if (!issymbol(p1))
		return;

	p1 = get_binding(p1);

	// must be two element vector

	if (!istensor(p1) || p1->u.tensor->ndim != 1 || p1->u.tensor->nelem != 2)
		return;

	push(p1->u.tensor->elem[0]);
	eval();
	yyfloat();
	eval();
	p2 = pop();

	push(p1->u.tensor->elem[1]);
	eval();
	yyfloat();
	eval();
	p3 = pop();

	if (!isnum(p2) || !isnum(p3))
		return;

	push(p2);
	xmin = pop_double();

	push(p3);
	xmax = pop_double();

	if (xmin == xmax)
		stop("draw: xrange is zero");
}

//-----------------------------------------------------------------------------
//
//	Example: yrange=(-10,10)
//
//	y	d	v (vertical pixel coordinate)
//
//	10	0.00	0
//
//	5	0.25	100
//
//	0	0.50	200
//
//	-5	0.75	300
//
//	-10	1.00	400
//
//	We have
//
//		d = (10 - y) / 20
//
//	          = (B - y) / (B - A)
//
//	where yrange=(A,B)
//
//	To convert d to v, multiply by N where N = 400.
//
//-----------------------------------------------------------------------------

void
setup_yrange(void)
{
	save();
	setup_yrange_f();
	restore();
}

void
setup_yrange_f(void)
{
	// default range is (-10,10)

	ymin = -10.0;

	ymax = 10.0;

	p1 = usr_symbol("yrange");

	if (!issymbol(p1))
		return;

	p1 = get_binding(p1);

	// must be two element vector

	if (!istensor(p1) || p1->u.tensor->ndim != 1 || p1->u.tensor->nelem != 2)
		return;

	push(p1->u.tensor->elem[0]);
	eval();
	yyfloat();
	eval();
	p2 = pop();

	push(p1->u.tensor->elem[1]);
	eval();
	yyfloat();
	eval();
	p3 = pop();

	if (!isnum(p2) || !isnum(p3))
		return;

	push(p2);
	ymin = pop_double();

	push(p3);
	ymax = pop_double();

	if (ymin == ymax)
		stop("draw: yrange is zero");
}

#define XOFF 0
#define YOFF 24

static void emit_xaxis(void);
static void emit_yaxis(void);

static void get_xzero(void);
static void get_yzero(void);

static int xzero, yzero;

void
emit_graph(void)
{
  int i, x, y;
  Bdisp_AllClr_VRAM();
  get_xzero();
  get_yzero();
  emit_xaxis();
  emit_yaxis();
  for (i = 0; i < draw_count; i++) {
          x = draw_buf[i].x;
          y = DIMY - draw_buf[i].y; // flip the y coordinate
          if (x < 0 || x > DIMX)
                  continue;
          if (y < 0 || y > DIMY)
                  continue;
          plot(x+XOFF, y+YOFF, COLOR_BLACK);
  }
  set_has_drawn(1);
}

static void
emit_xaxis(void)
{
        int x, y, x2, y2;

        if (yzero < 0 || yzero > DIMY)
                return;

        x = XOFF;
        y = YOFF + yzero;

        x2 = XOFF + DIMX;
        y2 = YOFF + yzero;

        drawLine(x, y, x2, y2, COLOR_BLACK);
}

static void
emit_yaxis(void)
{
        int x, y, x2, y2;

        if (xzero < 0 || xzero > DIMX)
                return;

        x = XOFF + xzero;
        y = YOFF;

        x2 = XOFF + xzero;
        y2 = YOFF + DIMY;

        drawLine(x, y, x2, y2, COLOR_BLACK);
}

static void
get_xzero(void)
{
        double x;
        x = -((double) DIMX) * xmin / (xmax - xmin) + 0.5;
        if (x < -10000.0)
                x = -10000.0;
        if (x > 10000.0)
                x = 10000.0;
        xzero = (int) x;
}

static void
get_yzero(void)
{
        double y;
        y = -((double) DIMY) * ymin / (ymax - ymin) + 0.5;
        if (y < -10000.0)
                y = -10000.0;
        if (y > 10000.0)
                y = 10000.0;
        yzero = DIMY - (int) y; // flip the y coordinate
}
/*
#define XOFF 0
#define YOFF 0

#define SHIM 10

static int k;
static unsigned char *buf;

static void emit_box(void);
static void emit_xaxis(void);
static void emit_yaxis(void);
static void emit_xscale(void);
static void emit_yscale(void);
static void emit_xzero(void);
static void emit_yzero(void);
static void get_xzero(void);
static void get_yzero(void);

static int xzero, yzero;

void
emit_graph(void)
{
	int h, i, len, x, y;

	get_xzero();
	get_yzero();

	len = 1000 + 5 * draw_count;

	buf = (unsigned char *) malloc(len);

	h = DIM + SHIM + text_metric[SMALL_FONT].ascent + text_metric[SMALL_FONT].descent;

	//buf[0] = (unsigned char) (h >> 8);
	//buf[1] = (unsigned char) h;

	//buf[2] = (unsigned char) (DIM >> 8);
	//buf[3] = (unsigned char) DIM;

	k = 0;

	emit_box();

	emit_xaxis();
	emit_yaxis();

	emit_xscale();
	emit_yscale();

	emit_xzero();
	emit_yzero();

	for (i = 0; i < draw_count; i++) {
		x = draw_buf[i].x;
		y = DIM - draw_buf[i].y; // flip the y coordinate
		if (x < 0 || x > DIM)
			continue;
		if (y < 0 || y > DIM)
			continue;
		x += XOFF;
		y += YOFF;
		buf[k++] = DRAW_POINT;
		buf[k++] = (unsigned char) (x >> 8);
		buf[k++] = (unsigned char) x;
		buf[k++] = (unsigned char) (y >> 8);
		buf[k++] = (unsigned char) y;
	}

	buf[k++] = 0;

	shipout(buf, DIM + 1, h);
}

static void
get_xzero(void)
{
	double x;
	x = -((double) DIM) * xmin / (xmax - xmin) + 0.5;
	if (x < -10000.0)
		x = -10000.0;
	if (x > 10000.0)
		x = 10000.0;
	xzero = (int) x;
}

static void
get_yzero(void)
{
	double y;
	y = -((double) DIM) * ymin / (ymax - ymin) + 0.5;
	if (y < -10000.0)
		y = -10000.0;
	if (y > 10000.0)
		y = 10000.0;
	yzero = DIM - (int) y; // flip the y coordinate
}

static void
emit_box(void)
{
	int x, y;

	buf[k++] = DRAW_BOX;

	x = XOFF;
	y = YOFF;
	buf[k++] = (unsigned char) (x >> 8);
	buf[k++] = (unsigned char) x;
	buf[k++] = (unsigned char) (y >> 8);
	buf[k++] = (unsigned char) y;

	x = XOFF + DIM;
	y = YOFF + DIM;
	buf[k++] = (unsigned char) (x >> 8);
	buf[k++] = (unsigned char) x;
	buf[k++] = (unsigned char) (y >> 8);
	buf[k++] = (unsigned char) y;
}

static void
emit_xaxis(void)
{
	int x, y;

	if (yzero < 0 || yzero > DIM)
		return;

	buf[k++] = DRAW_LINE;

	x = XOFF;
	y = YOFF + yzero;

	buf[k++] = (unsigned char) (x >> 8);
	buf[k++] = (unsigned char) x;
	buf[k++] = (unsigned char) (y >> 8);
	buf[k++] = (unsigned char) y;

	x = XOFF + DIM;
	y = YOFF + yzero;

	buf[k++] = (unsigned char) (x >> 8);
	buf[k++] = (unsigned char) x;
	buf[k++] = (unsigned char) (y >> 8);
	buf[k++] = (unsigned char) y;
}

static void
emit_yaxis(void)
{
	int x, y;

	if (xzero < 0 || xzero > DIM)
		return;

	buf[k++] = DRAW_LINE;

	x = XOFF + xzero;
	y = YOFF;

	buf[k++] = (unsigned char) (x >> 8);
	buf[k++] = (unsigned char) x;
	buf[k++] = (unsigned char) (y >> 8);
	buf[k++] = (unsigned char) y;

	x = XOFF + xzero;
	y = YOFF + DIM;

	buf[k++] = (unsigned char) (x >> 8);
	buf[k++] = (unsigned char) x;
	buf[k++] = (unsigned char) (y >> 8);
	buf[k++] = (unsigned char) y;
}

static void emit_xscale_f(int, char *);

static void
emit_xscale(void)
{
	static char s[100];
	sprintf(s, "%g", xmin);
	emit_xscale_f(0, s);
	sprintf(s, "%g", xmax);
	emit_xscale_f(DIM, s);
}

static void
emit_xscale_f(int xx, char *s)
{
	int d, i, len, w, x, y;

	// want to center the number w/o sign

	w = text_width(SMALL_FONT, s);

	if (*s == '-')
		d = w - text_width(SMALL_FONT, s + 1);
	else
		d = 0;

	x = XOFF + xx - (w - d) / 2 - d;
	y = YOFF + DIM + SHIM;

	buf[k++] = SMALL_FONT;
	buf[k++] = (unsigned char) (x >> 8);
	buf[k++] = (unsigned char) x;
	buf[k++] = (unsigned char) (y >> 8);
	buf[k++] = (unsigned char) y;

	len = (int) strlen(s);

	buf[k++] = (unsigned char) len;

	for (i = 0; i < len; i++)
		buf[k++] = (unsigned char) s[i];
}

static void emit_yscale_f(int, char *);

static void
emit_yscale(void)
{
	static char s[100];
	sprintf(s, "%g", ymax);
	emit_yscale_f(0, s);
	sprintf(s, "%g", ymin);
	emit_yscale_f(DIM, s);
}

static void
emit_yscale_f(int yy, char *s)
{
	int i, len, w, x, y;

	w = text_width(SMALL_FONT, s);

	x = XOFF - SHIM - w;
	y = YOFF + yy - text_metric[SMALL_FONT].ascent / 2;

	buf[k++] = SMALL_FONT;
	buf[k++] = (unsigned char) (x >> 8);
	buf[k++] = (unsigned char) x;
	buf[k++] = (unsigned char) (y >> 8);
	buf[k++] = (unsigned char) y;

	len = (int) strlen(s);

	buf[k++] = (unsigned char) len;

	for (i = 0; i < len; i++)
		buf[k++] = (unsigned char) s[i];
}

// emit the '0' axis label

// make sure it doesn't hit the other labels

static void
emit_xzero(void)
{
	int x, y;

	if (xzero < DIM / 4 || xzero > 3 * DIM / 4)
		return;

	x = XOFF + xzero - text_width(SMALL_FONT, "0") / 2;
	y = YOFF + DIM + SHIM;

	buf[k++] = SMALL_FONT;
	buf[k++] = (unsigned char) (x >> 8);
	buf[k++] = (unsigned char) x;
	buf[k++] = (unsigned char) (y >> 8);
	buf[k++] = (unsigned char) y;
	buf[k++] = 1;
	buf[k++] = '0';
}

// emit the '0' axis label

// make sure it doesn't hit the other labels

static void
emit_yzero(void)
{
	int x, y;

	if (yzero < DIM / 4 || yzero > 3 * DIM / 4)
		return;

	x = XOFF - SHIM - text_width(SMALL_FONT, "0");
	y = YOFF + yzero - text_metric[SMALL_FONT].ascent / 2;

	buf[k++] = SMALL_FONT;
	buf[k++] = (unsigned char) (x >> 8);
	buf[k++] = (unsigned char) x;
	buf[k++] = (unsigned char) (y >> 8);
	buf[k++] = (unsigned char) y;
	buf[k++] = 1;
	buf[k++] = '0';
}
*/