#ifndef STRAYNEKO_BED_H
#define STRAYNEKO_BED_H

#include <X11/Xlib.h>

#include <stdbool.h>

typedef struct {
    int x, y;
    int going_to_bed;
    Bool enabled;
    int dragging;
    int drag_offset_x, drag_offset_y;
    GC gc;
    Pixmap pixmap;
    Pixmap mask;
} BedData;

extern BedData Bed;

/* Validate a pending X11 bed-window position. Returns False when the
 * requested position is outside the allowed monitor and restores the
 * position of the already-visible window in changes. */
Bool BedValidateWindowPosition(Display *display, Window window,
                               XWindowChanges *changes);

#endif
