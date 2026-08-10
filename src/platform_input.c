#include "platform_input.h"

#include "bed.h"
#include "display.h"
#include "strayneko.h"
#include "x11_window.h"

#include <X11/Xlib.h>

void
ProcessPlatformEvent(const PlatformEvent *event)
{
    if (!event) {
        return;
    }

    switch (event->type) {
    case PLATFORM_EVENT_QUIT:
        TerminationRequested = 1;
        break;

    case PLATFORM_EVENT_REDRAW:
        DrawNeko(Neko.x, Neko.y, Neko.tick_count);
        break;

    case PLATFORM_EVENT_WINDOW_RAISE:
        X11WindowRaise(theDisplay, theWindow);
        break;

    case PLATFORM_EVENT_BED_DRAG_START:
        Bed.dragging = 1;
        Bed.drag_offset_x = event->x;
        Bed.drag_offset_y = event->y;
        break;

    case PLATFORM_EVENT_BED_DRAG_MOVE:
        if (Bed.dragging) {
            XWindowChanges changes;
            changes.x = event->x - Bed.drag_offset_x;
            changes.y = event->y - Bed.drag_offset_y;

            if (BedValidateWindowPosition(theDisplay, BedWindow, &changes)) {
                Bed.x = changes.x;
                Bed.y = changes.y;
                XConfigureWindow(theDisplay, BedWindow,
                                 CWX | CWY, &changes);
            }
        }
        break;

    case PLATFORM_EVENT_BED_DRAG_END:
        Bed.dragging = 0;
        break;

    case PLATFORM_EVENT_NONE:
    default:
        break;
    }
}
