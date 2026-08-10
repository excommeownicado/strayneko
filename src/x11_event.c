#include "x11_event.h"

int
X11EventPending(Display *display)
{
    return XPending(display);
}

void
X11EventNext(Display *display, XEvent *event)
{
    XNextEvent(display, event);
}

void
X11EventSelectInput(Display *display, Window window, long event_mask)
{
    XSelectInput(display, window, event_mask);
}

int
X11GrabPointer(Display *display, Window window, unsigned int event_mask)
{
    return XGrabPointer(display, window, False,
                        event_mask,
                        GrabModeAsync, GrabModeAsync,
                        None, None, CurrentTime);
}

void
X11UngrabPointer(Display *display)
{
    XUngrabPointer(display, CurrentTime);
}

int
X11QueryPointer(Display *display, Window window,
                Window *root_return, Window *child_return,
                int *root_x_return, int *root_y_return,
                int *win_x_return, int *win_y_return,
                unsigned int *mask_return)
{
    return XQueryPointer(display, window,
                         root_return, child_return,
                         root_x_return, root_y_return,
                         win_x_return, win_y_return,
                         mask_return);
}
