#ifndef STRAYNEKO_X11_WINDOW_COMPAT_H
#define STRAYNEKO_X11_WINDOW_COMPAT_H

#include "x11_window.h"

/*
 * Temporary migration bridge for the legacy X11 code.
 * New code should call the X11Window* API directly.
 */
#define XMapWindow(display, window) X11WindowMap((display), (window))
#define XUnmapWindow(display, window) X11WindowUnmap((display), (window))
#define XRaiseWindow(display, window) X11WindowRaise((display), (window))

#endif
