// plot a point to screen. by gbl08ma

#include "stdafx.h"
#include "defs.h"
#include "graphicsProvider.hpp"


void
eval_plot(void)
{
        push(cadr(p1));
        eval();
        push(caddr(p1));
        eval();
        eplot();
}

#define X p1
#define Y p2

void
eplot(void)
{
        save();

        X = pop();
        Y = pop();

        if (!(isnonnegativeinteger(X) && isnonnegativeinteger(Y))) {
                push_integer(0);
                restore();
                return;
        }
        double x,y;
        
        x = convert_rational_to_double(X);
        y = convert_rational_to_double(Y);
        
        int ix=(int)x, iy=(int)y;
        if(ix >= 0 && ix < LCD_WIDTH_PX && iy >= 0 && iy < LCD_HEIGHT_PX) {
          plot(ix, iy, COLOR_BLACK);
          push_integer(1);
        } else push_integer(0);

        restore();

}