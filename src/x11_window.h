#ifndef STRAYNEKO_X11_WINDOW_H
#define STRAYNEKO_X11_WINDOW_H

#include <X11/Xlib.h>

Window X11WindowCreate(
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
);

void X11WindowMove(Display *display, Window window, int x, int y);
void X11WindowRaise(Display *display, Window window);
void X11WindowMap(Display *display, Window window);
void X11WindowUnmap(Display *display, Window window);
void X11WindowDestroy(Display *display, Window window);
void X11WindowSync(Display *display);

#endif
