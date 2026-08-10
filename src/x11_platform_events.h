#ifndef STRAYNEKO_X11_PLATFORM_EVENTS_H
#define STRAYNEKO_X11_PLATFORM_EVENTS_H

#include "platform.h"

/* X11 backend implementation of the platform-neutral event API. */
bool PlatformPollEvent(PlatformEvent *event);

#endif
