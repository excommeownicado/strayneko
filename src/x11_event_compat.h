#ifndef STRAYNEKO_X11_EVENT_COMPAT_H
#define STRAYNEKO_X11_EVENT_COMPAT_H

#include "x11_event.h"

#define XPending(display) X11EventPending(display)
#define XNextEvent(display, event) X11EventNext(display, event)
#define XSelectInput(display, window, mask) X11EventSelectInput(display, window, mask)

#endif
