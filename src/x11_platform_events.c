#include "x11_platform_events.h"
#include "strayneko.h"
#include "x11_event.h"

#include <X11/keysym.h>

bool
PlatformPollEvent(PlatformEvent *event)
{
    XEvent xevent;

    if (!event || !X11EventPending(theDisplay)) {
        return false;
    }

    event->type = PLATFORM_EVENT_NONE;
    event->x = 0;
    event->y = 0;
    event->button = 0;

    X11EventNext(theDisplay, &xevent);

    switch (xevent.type) {
    case Expose:
        if (xevent.xexpose.count == 0) {
            if (xevent.xexpose.window == BedWindow) {
                event->type = PLATFORM_EVENT_BED_REDRAW;
            } else if (xevent.xexpose.window == theWindow) {
                event->type = PLATFORM_EVENT_REDRAW;
                event->x = xevent.xexpose.x;
                event->y = xevent.xexpose.y;
            }
            return true;
        }
        break;

    case KeyPress: {
        KeySym keysym = XLookupKeysym(&xevent.xkey, 0);

        if ((keysym == XK_q || keysym == XK_Q) &&
            (xevent.xkey.state & Mod1Mask)) {
            event->type = PLATFORM_EVENT_QUIT;
            return true;
        }
        break;
    }

    case VisibilityNotify:
        if (xevent.xvisibility.window == theWindow) {
            event->type = PLATFORM_EVENT_WINDOW_RAISE;
            return true;
        }
        break;

    case ButtonPress:
        if (xevent.xbutton.window == BedWindow &&
            xevent.xbutton.button == Button1) {
            event->type = PLATFORM_EVENT_BED_DRAG_START;
            event->x = xevent.xbutton.x_root;
            event->y = xevent.xbutton.y_root;
            event->button = xevent.xbutton.button;
            return true;
        }
        break;

    case MotionNotify:
        if (xevent.xmotion.window == BedWindow) {
            event->type = PLATFORM_EVENT_BED_DRAG_MOVE;
            event->x = xevent.xmotion.x_root;
            event->y = xevent.xmotion.y_root;
            return true;
        }
        break;

    case ButtonRelease:
        if (xevent.xbutton.window == BedWindow &&
            xevent.xbutton.button == Button1) {
            event->type = PLATFORM_EVENT_BED_DRAG_END;
            event->button = xevent.xbutton.button;
            return true;
        }
        break;

    default:
        break;
    }

    return true;
}
