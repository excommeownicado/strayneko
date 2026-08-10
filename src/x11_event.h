#ifndef STRAYNEKO_X11_EVENT_H
#define STRAYNEKO_X11_EVENT_H

#include <X11/Xlib.h>

int X11EventPending(Display *display);
void X11EventNext(Display *display, XEvent *event);
void X11EventSelectInput(Display *display, Window window, long event_mask);
int X11GrabPointer(Display *display, Window window, unsigned int event_mask);
void X11UngrabPointer(Display *display);
int X11QueryPointer(Display *display, Window window,
                    Window *root_return, Window *child_return,
                    int *root_x_return, int *root_y_return,
                    int *win_x_return, int *win_y_return,
                    unsigned int *mask_return);

#endif
