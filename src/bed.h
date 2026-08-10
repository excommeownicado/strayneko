#ifndef STRAYNEKO_BED_H
#define STRAYNEKO_BED_H

#include <X11/Xlib.h>

/* Validate a pending X11 bed-window position. Returns False when the
 * requested position is outside the allowed monitor and restores the
 * position of the already-visible window in changes. */
Bool BedValidateWindowPosition(Display *display, Window window,
                               XWindowChanges *changes);

#endif
