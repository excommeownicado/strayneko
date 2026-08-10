#include "x11_platform_events.h"
#include "x11_event.h"

#include <X11/keysym.h>

bool
X11PollPlatformEvent(Display *display, Window neko_window,
                     Window bed_window, PlatformEvent *event)
{
    XEvent xevent;

    if (!event || !X11EventPending(display)) {
        return false;
    }

    event->type = PLATFORM_EVENT_NONE;
    event->x = 0;
    event->y = 0;
    event->button = 0;

    X11EventNext(display, &xevent);

    switch (xevent.type) {
    case Expose:
        if (xevent.xexpose.count == 0) {
            event->type = PLATFORM_EVENT_REDRAW;
            event->x = xevent.xexpose.x;
            event->y = xevent.xexpose.y;
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
        if (xevent.xvisibility.window == neko_window) {
            event->type = PLATFORM_EVENT_WINDOW_RAISE;
            return true;
        }
        break;

    case ButtonPress:
        if (xevent.xbutton.window == bed_window &&
            xevent.xbutton.button == Button1) {
            /* Keep drag coordinates in the same root coordinate system as
             * MotionNotify. Using x/y here makes the initial offset relative
             * to the bed, which causes the bed to jump/stick at an edge. */
            event->type = PLATFORM_EVENT_BED_DRAG_START;
            event->x = xevent.xbutton.x_root;
            event->y = xevent.xbutton.y_root;
            event->button = xevent.xbutton.button;
            return true;
        }
        break;

    case MotionNotify:
        if (xevent.xmotion.window == bed_window) {
            event->type = PLATFORM_EVENT_BED_DRAG_MOVE;
            event->x = xevent.xmotion.x_root;
            event->y = xevent.xmotion.y_root;
            return true;
        }
        break;

    case ButtonRelease:
        if (xevent.xbutton.window == bed_window &&
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
