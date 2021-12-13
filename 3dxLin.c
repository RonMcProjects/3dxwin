/* 
 * gcc 3dxLin.c -Wall -lm -lX11 -o 3dxLin
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * Constants
 */
#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 192

/*
 * Globals
 */
int p = 0, q = 0, xp = 0, yp = 0, yr = 0, zp = 0, xl = 0, xi = 0, xx = 0, i = 0;
double m[257][2], xr = 0.0, xf = 0.0, yf = 0.0, zf = 0.0, zi = 0.0, zt = 0.0,
       zz = 0.0, xt = 0.0, yy = 0.0, t = 0.0, x2 = 0.0, y2 = 0.0, x1 = 0.0,
       y_one = 0.0, x_scale, y_scale, x_offset, y_offset;

int subr(Display *display, int screen, Window window)
{
    t = (yy - zz) * 0.9;
    i = (int)(((double)xx + zz + (double)p) * 0.7);
    if (i > (WINDOW_WIDTH - 1))
        return(0);
    if (m[i][1] == 0.0 && m[i][0] == 0.0)
    {
        m[i][1] = t;
        m[i][0] = t;
    }
    if (t >= m[i][0])
        m[i][0] = t;
    else if (t <= m[i][1])
        m[i][1] = t;
    else
    {
        x2 = 0.0;
        y2 = 0.0;
        return(0);
    }
    x1 = ((double)xx + zz + (double)p) * 0.7;
    y_one = (yy - zz) * 0.9 + (double)q + 20.0;
    if (y_one == 0.0)
    {
        x2 = 0.0;
        y2 = 0.0;
        return(0);
    }
    if (x1 > (double)(WINDOW_WIDTH - 1) || y2 > (double)(WINDOW_HEIGHT - 1))
    {
        x2 = 0.0;
        y2 = 0.0;
    }
    if (x2 == 0.0 || y2 == 0.0)
    {
        x2 = x1;
        y2 = y_one;
        return(0);
    }
    if (y_one < 0.0 || y_one > (double)(WINDOW_HEIGHT - 1))
    {
        x2 = 0.0;
        y2 = 0.0;
        return(0);
    }

    (void)XDrawLine(display, window, DefaultGC(display, screen),
                    (int)(x2 * x_scale) + (int)x_offset, (int)y_offset - (int)(y2 * y_scale),
                    (int)(x1 * x_scale) + (int)x_offset, (int)y_offset - (int)(y_one * y_scale));
    XFlush(display);

    x2 = x1;
    y2 = y_one;
    return(0);
}

int display_wave(Display *display, int screen, Window window)
{
    int n;

    for (n = 0; n < 257; n++)
    {
        m[n][0] = 0.0;
        m[n][1] = 0.0;
    }

    x_scale = (double)WINDOW_WIDTH / 256.0;
    y_scale = (double)WINDOW_HEIGHT / 210.0;
    x_offset = (double)WINDOW_WIDTH / 15.0;
    y_offset = (double)WINDOW_HEIGHT - 4.0;

    p = 160;
    q = 100;
    xp = 144;
    xr = 1.5 * 3.1415927;
    yp = 56;
    yr = 1;
    zp = 64;
    xf = xr / (double)xp;
    yf = (double)yp / (double)yr;
    zf = xr / (double)zp;
    for (zi = (double)(q - 1); zi >= (double)(-q); zi -= 1.5)
    {
        if (zi >= (double)(-zp) && zi <= (double)zp)
        {
            zt = zi * (double)xp / (double)zp;
            zz = zi;
            xl = (int)(0.5 + sqrt((double)(xp * xp) - (zt * zt)));
            for (xi = -xl; xi <= xl; xi++)
            {
                xt = sqrt((double)(xi * xi) + (zt * zt)) * xf;
                xx = xi;
                yy = (cos(xt) + 0.4 * cos(3.0 * xt)) * yf;
                (void)subr(display, screen, window);
            }
        }
        x2 = 0.0;
        y2 = 0.0;
    }
    XFlush(display);

    return(0);
}

int main(argc, argv)
int argc;
char **argv;
{
    Display *display;
    Window window;
    XEvent event;
    int screen;

    display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, WINDOW_WIDTH, WINDOW_HEIGHT, 1,
                            BlackPixel(display, screen), WhitePixel(display, screen));
    XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask);
    XMapWindow(display, window);

    while(1)
    {
        XNextEvent(display, &event);
        if (event.type == Expose)
        {
            display_wave(display, screen, window);
        }
        if ((event.type == KeyPress) || (event.type == ButtonPress))
            break;
    }

    XCloseDisplay(display);
    return(0);
}
