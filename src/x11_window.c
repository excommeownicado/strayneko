#include "x11_window.h"

Window
X11WindowCreate(
    Display *display,
    Window parent,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    unsigned int border_width,
    int depth,
    unsigned int class,
    Visual *visual,
    unsigned long valuemask,
    XSetWindowAttributes *attributes
)
{
    return XCreateWindow(
        display,
        parent,
        x,
        y,
        width,
        height,
        border_width,
        depth,
        class,
        visual,
        valuemask,
        attributes
    );
}

void
X11WindowMove(Display *display, Window window, int x, int y)
{
    XMoveWindow(display, window, x, y);
}

void
X11WindowConfigure(Display *display, Window window,
                   unsigned int value_mask, XWindowChanges *changes)
{
    if (!changes) {
        return;
    }

    XConfigureWindow(display, window, value_mask, changes);
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
