#include "x11_window.h"

void
X11WindowMove(Display *display, Window window, int x, int y)
{
    XMoveWindow(display, window, x, y);
}

void
X11WindowRaise(Display *display, Window window)
{
    XRaiseWindow(display, window);
}

void
X11WindowMap(Display *display, Window window)
{
    XMapWindow(display, window);
}

void
X11WindowUnmap(Display *display, Window window)
{
    XUnmapWindow(display, window);
}

void
X11WindowDestroy(Display *display, Window window)
{
    if (window != None) {
        XDestroyWindow(display, window);
    }
}

void
X11WindowSync(Display *display)
{
    XSync(display, False);
}
