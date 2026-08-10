#ifndef STRAYNEKO_X11_WINDOW_COMPAT_H
#define STRAYNEKO_X11_WINDOW_COMPAT_H

#include "x11_window.h"

/*
 * Temporary migration bridge for the legacy X11 code.
 * New code should call the X11Window* API directly.
 */
#define XCreateWindow(display, parent, x, y, width, height, border_width, depth, class, visual, valuemask, attributes) \
    X11WindowCreate((display), (parent), (x), (y), (width), (height), (border_width), (depth), (class), (visual), (valuemask), (attributes))
#define XMapWindow(display, window) X11WindowMap((display), (window))
#define XUnmapWindow(display, window) X11WindowUnmap((display), (window))
#define XRaiseWindow(display, window) X11WindowRaise((display), (window))
#define XDestroyWindow(display, window) X11WindowDestroy((display), (window))
#define XConfigureWindow(display, window, value_mask, changes) \
    X11WindowConfigure((display), (window), (value_mask), (changes))
#define XSync(display, discard) X11WindowSync((display))

#endif
