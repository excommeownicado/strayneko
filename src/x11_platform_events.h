#ifndef STRAYNEKO_X11_PLATFORM_EVENTS_H
#define STRAYNEKO_X11_PLATFORM_EVENTS_H

#include "platform_event.h"

#include <X11/Xlib.h>

/* Translate one pending X11 event into a platform-neutral event.
 * Returns false when there is no event to consume. */
bool X11PollPlatformEvent(Display *display, Window neko_window,
                           Window bed_window, PlatformEvent *event);

#endif
