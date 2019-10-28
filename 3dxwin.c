
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 375

/*
 * Constants
 */
char WINDOW_NAME[] = __FILE__;
char ICON_NAME[] = __FILE__;
char RED[] = "Red";		/* added for color. */
char BLUE[] = "Blue";		/* added for color. */

/*
 * Globals
 */
Display *display;
int screen;
Window main_window;
GC gc;
unsigned long foreground, background;
XColor RGB_color, hardware_color;	/* added for color. */
Colormap color_map;		/* added for color. */

int p = 0, q = 0, xp = 0, yp = 0, yr = 0, zp = 0, xl = 0, xi = 0, xx = 0,
  i = 0;
double m[257][2], xr = 0.0, xf = 0.0, yf = 0.0, zf = 0.0, zi = 0.0, zt = 0.0,
  zz = 0.0, xt = 0.0, yy = 0.0, t = 0.0, x2 = 0.0, y2 = 0.0, x1 = 0.0,
  y_one = 0.0, x_scale, y_scale, x_offset, y_offset;

/*
 * Connect to the server and get the display device
 * and the screen number.
 */
void
initX ()
{
  int depth;			/* depth of the color map. */

  /*
   * Set the display name from the environment variable DISPLAY.
   */
  display = XOpenDisplay (NULL);
  if (display == NULL)
    {
      fprintf (stderr, "Unable to open display %s\n", XDisplayName (NULL));
      exit (1);
    }
  screen = DefaultScreen (display);

  /* Find the depth of the colour map. */
  depth = DefaultDepth (display, screen);

  /* Set the default foreground and background, in case we cannot use colour. */
  foreground = BlackPixel (display, screen);
  background = WhitePixel (display, screen);

  if (depth > 1)		/* not monochrome */
    {
      color_map = DefaultColormap (display, screen);
      if (XLookupColor (display, color_map, "white", &RGB_color, &hardware_color) != 0
	  &&
	  XAllocColor (display, color_map, &hardware_color) != 0)
	background = hardware_color.pixel;

      if (XLookupColor (display, color_map, "black", &RGB_color, &hardware_color) != 0
	  &&
	  XAllocColor (display, color_map, &hardware_color) != 0)
	foreground = hardware_color.pixel;

    }
}

/*
 * Opens a window on the display device, and returns
 * the windows ID.
 */
Window
openWindow (x, y, width, height,
	    border_width, argc, argv)
     int x, y,			/* co-ords of the upper left corner in pixels. */
       width, height,		/* size of the windows in pixels. */
       border_width;		/* the border width is not included in the other dimensions. */
     int argc;
     char **argv;
{
  Window new_window;
  XSizeHints size_hints;

  /* now create the window. */
  new_window = XCreateSimpleWindow (display,
				    DefaultRootWindow (display),
				    x, y, width, height,
				    border_width,
				    foreground, background);

  /* set up the size hints for the window manager. */
  size_hints.x = x;
  size_hints.y = y;
  size_hints.width = width;
  size_hints.height = height;

  size_hints.flags = PPosition | PSize;

  /* and state what hints are included. */
  XSetStandardProperties (display, new_window,
			  WINDOW_NAME, ICON_NAME,
			  None,	/* no icon map */
			  argv, argc, &size_hints);

  /* Return the window ID. */
  return (new_window);
}

/*
 * Create a graphics context using default values and,
 * return it in the pointer gc.
 */
GC
getGC ()
{
  GC gc;
  XGCValues gcValues;

  gc = XCreateGC (display, main_window,
		  (unsigned long) 0, &gcValues);

  XSetBackground (display, gc, background);
  XSetForeground (display, gc, foreground);

  return (gc);
}

/*
 * Terminate the program gracefully.
 */
void
quitX ()
{
  XCloseDisplay (display);
  exit (0);
}

int 
subr ()
{
  t = (yy - zz) * 0.9;
  i = (int) (((double) xx + zz + (double) p) * 0.7);
  if (i > 255)
    return (0);
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
      return (0);
    }
  x1 = ((double) xx + zz + (double) p) * 0.7;
  y_one = (yy - zz) * 0.9 + (double) q + 20.0;
  if (y_one == 0.0)
    {
      x2 = 0.0;
      y2 = 0.0;
      return (0);
    }
  if (x1 > 255.0 || y2 > 191.0)
    {
      x2 = 0.0;
      y2 = 0.0;
    }
  if (x2 == 0.0 || y2 == 0.0)
    {
      x2 = x1;
      y2 = y_one;
      return (0);
    }
  if (y_one < 0.0 || y_one > 191.0)
    {
      x2 = 0.0;
      y2 = 0.0;
      return (0);
    }

  (void) XDrawLine (display, main_window, gc,
		    (int) (x2 * x_scale) + (int) x_offset, (int) y_offset - (int) (y2 * y_scale),
		    (int) (x1 * x_scale) + (int) x_offset, (int) y_offset - (int) (y_one * y_scale));

  x2 = x1;
  y2 = y_one;
  return (0);
}

display_something ()
{
  int driver, mode, error, n;
  XEvent event;

  /* flush event queue */
  XSelectInput (display, main_window, ExposureMask);
  XNextEvent (display, &event);

  for (n = 0; n < 257; n++)
    {
      m[n][0] = 0.0;
      m[n][1] = 0.0;
    }

  x_scale = (double) WINDOW_WIDTH / 256.0;
  y_scale = (double) WINDOW_HEIGHT / 210.0;
  x_offset = (double) WINDOW_WIDTH / 10.0;
  y_offset = (double) WINDOW_HEIGHT;

  p = 160;
  q = 100;
  xp = 144;
  xr = 1.5 * 3.1415927;
  yp = 56;
  yr = 1;
  zp = 64;
  xf = xr / (double) xp;
  yf = (double) yp / (double) yr;
  zf = xr / (double) zp;
  for (zi = (double) (q - 1); zi >= (double) (-q); zi -= 1.5)
    {
      if (zi >= (double) (-zp) && zi <= (double) zp)
	{
	  zt = zi * (double) xp / (double) zp;
	  zz = zi;
	  xl = (int) (0.5 + sqrt ((double) (xp * xp) - (zt * zt)));
	  for (xi = -xl; xi <= xl; xi++)
	    {
	      xt = sqrt ((double) (xi * xi) + (zt * zt)) * xf;
	      xx = xi;
	      yy = (cos (xt) + 0.4 * cos (3.0 * xt)) * yf;
	      (void) subr ();
	    }
	}
      x2 = 0.0;
      y2 = 0.0;
    }
  XFlush (display);

  return (0);
}

int
main (argc, argv)
     int argc;
     char **argv;
{
  int done;
  XEvent event;

  initX ();
  main_window = openWindow (10, 20, WINDOW_WIDTH, WINDOW_HEIGHT, 5, argc, argv);
  gc = getGC ();

  /* Display the window on the screen. */
  XMapWindow (display, main_window);

  display_something ();

  XSelectInput (display, main_window, KeyPressMask | ButtonPressMask);

  done = 0;
  while (!done)
    {
      XNextEvent (display, &event);
      if ((event.type == KeyPress) || (event.type == ButtonPress))
	done = 1;
    }

  /*  sleep( 10 ); */

  quitX ();
}
