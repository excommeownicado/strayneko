#include "strayneko.h"

static const SpriteData SpriteDataTable[SPRITE_COUNT] = {
    [SPRITE_MATI2] = { mati2_bits, mati2_mask_bits },
    [SPRITE_JARE2] = { jare2_bits, jare2_mask_bits },
    [SPRITE_KAKI1] = { kaki1_bits, kaki1_mask_bits },
    [SPRITE_KAKI2] = { kaki2_bits, kaki2_mask_bits },
    [SPRITE_MATI3] = { mati3_bits, mati3_mask_bits },
    [SPRITE_SLEEP1] = { sleep1_bits, sleep1_mask_bits },
    [SPRITE_SLEEP2] = { sleep2_bits, sleep2_mask_bits },
    [SPRITE_AWAKE] = { awake_bits, awake_mask_bits },
    [SPRITE_UP1] = { up1_bits, up1_mask_bits },
    [SPRITE_UP2] = { up2_bits, up2_mask_bits },
    [SPRITE_DOWN1] = { down1_bits, down1_mask_bits },
    [SPRITE_DOWN2] = { down2_bits, down2_mask_bits },
    [SPRITE_LEFT1] = { left1_bits, left1_mask_bits },
    [SPRITE_LEFT2] = { left2_bits, left2_mask_bits },
    [SPRITE_RIGHT1] = { right1_bits, right1_mask_bits },
    [SPRITE_RIGHT2] = { right2_bits, right2_mask_bits },
    [SPRITE_UPLEFT1] = { upleft1_bits, upleft1_mask_bits },
    [SPRITE_UPLEFT2] = { upleft2_bits, upleft2_mask_bits },
    [SPRITE_UPRIGHT1] = { upright1_bits, upright1_mask_bits },
    [SPRITE_UPRIGHT2] = { upright2_bits, upright2_mask_bits },
    [SPRITE_DOWNLEFT1] = { dwleft1_bits, dwleft1_mask_bits },
    [SPRITE_DOWNLEFT2] = { dwleft2_bits, dwleft2_mask_bits },
    [SPRITE_DOWNRIGHT1] = { dwright1_bits, dwright1_mask_bits },
    [SPRITE_DOWNRIGHT2] = { dwright2_bits, dwright2_mask_bits },
    [SPRITE_UPTOGI1] = { utogi1_bits, utogi1_mask_bits },
    [SPRITE_UPTOGI2] = { utogi2_bits, utogi2_mask_bits },
    [SPRITE_DOWNTOGI1] = { dtogi1_bits, dtogi1_mask_bits },
    [SPRITE_DOWNTOGI2] = { dtogi2_bits, dtogi2_mask_bits },
    [SPRITE_LEFTTOGI1] = { ltogi1_bits, ltogi1_mask_bits },
    [SPRITE_LEFTTOGI2] = { ltogi2_bits, ltogi2_mask_bits },
    [SPRITE_RIGHTTOGI1] = { rtogi1_bits, rtogi1_mask_bits },
    [SPRITE_RIGHTTOGI2] = { rtogi2_bits, rtogi2_mask_bits }
};

Sprite Sprites[SPRITE_COUNT] = {0};

static const SpriteID AnimationPattern[][2] = {
    {     SPRITE_MATI2,       SPRITE_MATI2        },
    {     SPRITE_JARE2,       SPRITE_MATI2        },
    {     SPRITE_KAKI1,       SPRITE_KAKI2        },
    {     SPRITE_MATI3,       SPRITE_MATI3        },
    {     SPRITE_SLEEP1,      SPRITE_SLEEP2       },
    {     SPRITE_AWAKE,       SPRITE_AWAKE        },
    {     SPRITE_UP1,         SPRITE_UP2          },
    {     SPRITE_DOWN1,       SPRITE_DOWN2        },
    {     SPRITE_LEFT1,       SPRITE_LEFT2        },
    {     SPRITE_RIGHT1,      SPRITE_RIGHT2       },
    {     SPRITE_UPLEFT1,     SPRITE_UPLEFT2      },
    {     SPRITE_UPRIGHT1,    SPRITE_UPRIGHT2     },
    {     SPRITE_DOWNLEFT1,   SPRITE_DOWNLEFT2    },
    {     SPRITE_DOWNRIGHT1,  SPRITE_DOWNRIGHT2   },
    {     SPRITE_UPTOGI1,     SPRITE_UPTOGI2      },
    {     SPRITE_DOWNTOGI1,   SPRITE_DOWNTOGI2    },
    {     SPRITE_LEFTTOGI1,   SPRITE_LEFTTOGI2    },
    {     SPRITE_RIGHTTOGI1,  SPRITE_RIGHTTOGI2   },
};

MonitorRect
GetMonitorRect(int monitor)
{
    MonitorRect empty = {0};

    if (!Monitors ||
        monitor < 0 ||
        monitor >= MonitorCount) {
        return empty;
    }

    return (MonitorRect) {
        .x = Monitors[monitor].x,
        .y = Monitors[monitor].y,
        .width = Monitors[monitor].width,
        .height = Monitors[monitor].height
    };
}

int
RectOnMonitor(int x, int y, int w, int h)
{
    int i;

    if (MonitorCount <= 0) {
        return x >= 0 && y >= 0 &&
            x + w <= (int)WindowWidth &&
            y + h <= (int)WindowHeight;
    }

    if (Config.restrict_monitor >= 0 &&
        Config.restrict_monitor < MonitorCount) {

        MonitorRect rect = GetMonitorRect(Config.restrict_monitor);

        return x >= rect.x &&
            y >= rect.y &&
            x + w <= rect.x + rect.width &&
            y + h <= rect.y + rect.height;
    }

    for (i = 0; i < MonitorCount; i++) {
        MonitorRect rect = GetMonitorRect(i);

        if (x >= rect.x &&
            y >= rect.y &&
            x + w <= rect.x + rect.width &&
            y + h <= rect.y + rect.height) {
            return 1;
        }
    }

    return 0;
}

int
FindMonitorFor(int x, int y)
{
    for (int i = 0; i < MonitorCount; i++) {
        MonitorRect rect = GetMonitorRect(i);

        if (x >= rect.x &&
            y >= rect.y &&
            x < rect.x + rect.width &&
            y < rect.y + rect.height) {
            return i;
        }
    }

    return -1;
}

MonitorBounds
GetMonitorBounds(int monitor)
{
    MonitorRect rect = GetMonitorRect(monitor);

    MonitorBounds bounds;

    bounds.min_x = rect.x;
    bounds.min_y = rect.y;
    bounds.max_x = rect.width > BITMAP_WIDTH
        ? rect.x + rect.width - BITMAP_WIDTH
        : rect.x;
    bounds.max_y = rect.height > BITMAP_HEIGHT
        ? rect.y + rect.height - BITMAP_HEIGHT
        : rect.y;

    if (bounds.max_x < bounds.min_x) {
        bounds.max_x = bounds.min_x;
    }

    if (bounds.max_y < bounds.min_y) {
        bounds.max_y = bounds.min_y;
    }

    return bounds;
}

static void
CreateBedWindow(void)
{
    XSetWindowAttributes theWindowAttributes;
    unsigned long theWindowMask;

    theWindowAttributes.background_pixel = theBackgroundColor.pixel;
    theWindowAttributes.override_redirect = True;

    theWindowMask = CWBackPixel | CWOverrideRedirect;

    BedWindow = XCreateWindow(theDisplay, theRoot, Bed.x, Bed.y,
            bed_width, bed_height,
            0, theDepth, InputOutput, CopyFromParent,
            theWindowMask, &theWindowAttributes);
    Bed.pixmap = XCreateBitmapFromData(theDisplay, theRoot, bed_bits,
            bed_width, bed_height);
    Bed.mask = XCreateBitmapFromData(theDisplay, theRoot, bed_mask_bits,
            bed_mask_width, bed_mask_height);
#ifdef SHAPE
    if (!Config.no_shape) {
        XShapeCombineMask(theDisplay, BedWindow, ShapeBounding, 0, 0,
                Bed.mask, ShapeSet);
    }
#endif
    Bed.gc = XCreateGC(theDisplay, theRoot, 0, NULL);
    XSetForeground(theDisplay, Bed.gc, theForegroundColor.pixel);
    XSetBackground(theDisplay, Bed.gc, theBackgroundColor.pixel);
    XSetStipple(theDisplay, Bed.gc, Bed.pixmap);
    XSetFillStyle(theDisplay, Bed.gc, FillOpaqueStippled);
    XSelectInput(theDisplay, BedWindow,
            ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    XMapWindow(theDisplay, BedWindow);
    XFillRectangle(theDisplay, BedWindow, Bed.gc, 0, 0, bed_width, bed_height);
}

void
InitBitmapAndGCs(void)
{
    XGCValues theGCValues;

    theGCValues.function = GXcopy;
    theGCValues.foreground = theForegroundColor.pixel;
    theGCValues.background = theBackgroundColor.pixel;
    theGCValues.fill_style = FillTiled;
    theGCValues.ts_x_origin = 0;
    theGCValues.ts_y_origin = 0;

    for (int i = 0; i < SPRITE_COUNT; i++) {
        if (!SpriteDataTable[i].bits || !SpriteDataTable[i].mask_bits) {
            fprintf(stderr, "Missing sprite data %d\n", i);
            exit(1);
        }

        Sprites[i].pixmap = XCreatePixmapFromBitmapData(
            theDisplay,
            theRoot,
            (char *)SpriteDataTable[i].bits,
            BITMAP_WIDTH,
            BITMAP_HEIGHT,
            theForegroundColor.pixel,
            theBackgroundColor.pixel,
            theDepth
        );

        theGCValues.tile = Sprites[i].pixmap;

        Sprites[i].mask = XCreateBitmapFromData(
            theDisplay,
            theRoot,
            (char *)SpriteDataTable[i].mask_bits,
            BITMAP_WIDTH,
            BITMAP_HEIGHT
        );

        Sprites[i].gc = XCreateGC(
            theDisplay,
            theWindow,
            GCFunction |
            GCForeground |
            GCBackground |
            GCTile |
            GCTileStipXOrigin |
            GCTileStipYOrigin |
            GCFillStyle,
            &theGCValues
        );

        if (Sprites[i].pixmap == None ||
            Sprites[i].mask == None ||
            Sprites[i].gc == NULL) {
            fprintf(stderr, "%s: failed to create sprite %d\n",
                    ProgramName, i);
            exit(1);
        }
    }
}

void
DrawNeko(int x, int y, int tick)
{
    if (Neko.state < 0 ||
        Neko.state >= (int)(sizeof(AnimationPattern) / sizeof(AnimationPattern[0]))) {
        return;
    }

    tick &= 1;

    Sprite *sprite = &Sprites[AnimationPattern[Neko.state][tick]];
    GC DrawGC = sprite->gc;
    Pixmap DrawMask = sprite->mask;

    if ((x != Neko.last_x) || (y != Neko.last_y) || (DrawGC != Neko.last_gc)) {
        XWindowChanges theChanges;

        theChanges.x = x;
        theChanges.y = y;
        XConfigureWindow(theDisplay, theWindow, CWX | CWY, &theChanges);
#ifdef SHAPE
        if (Config.no_shape == False) {
            XShapeCombineMask(theDisplay, theWindow, ShapeBounding,
                              0, 0, DrawMask, ShapeSet);
        }
#endif
        if (DontMapped) {
            XMapWindow(theDisplay, theWindow);
            DontMapped = 0;
        }
        XFillRectangle(theDisplay, theWindow, DrawGC,
                       0, 0, BITMAP_WIDTH, BITMAP_HEIGHT);
    }

    XFlush(theDisplay);
    Neko.last_x = x;
    Neko.last_y = y;
    Neko.last_gc = DrawGC;
}

void
InitScreen(char *DisplayName)
{
    XSetWindowAttributes theWindowAttributes;
    unsigned long theWindowMask;
    Window theTempRoot;
    int WindowPointX;
    int WindowPointY;
    unsigned int BorderWidth;
    int event_base, error_base;

    if ((theDisplay = XOpenDisplay(DisplayName)) == NULL) {
        fprintf(stderr, "%s: Can't open display", ProgramName);
        if (DisplayName != NULL) {
            fprintf(stderr, " %s.\n", DisplayName);
        } else {
            fprintf(stderr, ".\n");
        }
        exit(1);
    }

    InitMonitors();

    if (Config.restrict_monitor < -1 ||
        Config.restrict_monitor >= MonitorCount) {
        fprintf(stderr, "%s: monitor %d is unavailable.\n",
                ProgramName, Config.restrict_monitor);
        exit(1);
    }

    GetResources();

    if (Synchronous == True) {
        fprintf(stderr, "Synchronizing.\n");
        XSynchronize(theDisplay, True);
    }

#ifdef SHAPE
    if (!Config.no_shape && XShapeQueryExtension(theDisplay,
                                         &event_base, &error_base) == False) {
        fprintf(stderr, "Display does not support shape extension.\n");
        Config.no_shape = True;
    }
#endif

    theScreen = DefaultScreen(theDisplay);
    theDepth = DefaultDepth(theDisplay, theScreen);

    theRoot = RootWindow(theDisplay, theScreen);

    XGetGeometry(theDisplay, theRoot, &theTempRoot,
                 &WindowPointX, &WindowPointY,
                 &WindowWidth, &WindowHeight,
                 &BorderWidth, &theDepth);

    if (Bed.enabled) {
        if (!LoadBedPosition() || !RectOnMonitor(Bed.x, Bed.y, bed_width, bed_height)) {
            PlaceBedOnMonitor();
        }
    }

    SetupColors();

    theWindowAttributes.background_pixel = theBackgroundColor.pixel;
    theWindowAttributes.override_redirect = True;

    theWindowMask = CWBackPixel | CWOverrideRedirect;

    theWindow = XCreateWindow(theDisplay, theRoot, 0, 0,
                              BITMAP_WIDTH, BITMAP_HEIGHT,
                              0, theDepth, InputOutput, CopyFromParent,
                              theWindowMask, &theWindowAttributes);
    InitBitmapAndGCs();

    XSelectInput(theDisplay, theWindow,
                 ExposureMask | VisibilityChangeMask | KeyPressMask);

    if (Bed.enabled) {
        CreateBedWindow();
    }

    XFlush(theDisplay);
}

void
RestoreCursor(void)
{
    XSetWindowAttributes theWindowAttributes;

    theWindowAttributes.cursor = None;
    XChangeWindowAttributes(theDisplay, theRoot, CWCursor,
                             &theWindowAttributes);

    for (int i = 0; i < SPRITE_COUNT; i++) {
        if (Sprites[i].pixmap != None)
            XFreePixmap(theDisplay, Sprites[i].pixmap);

        if (Sprites[i].mask != None)
            XFreePixmap(theDisplay, Sprites[i].mask);

        if (Sprites[i].gc != NULL)
            XFreeGC(theDisplay, Sprites[i].gc);
    }

    if (Bed.enabled) {
        if (Bed.pixmap != None)
            XFreePixmap(theDisplay, Bed.pixmap);

        if (Bed.mask != None)
            XFreePixmap(theDisplay, Bed.mask);

        if (Bed.gc != NULL)
            XFreeGC(theDisplay, Bed.gc);
    }

    if (Monitors) {
        XRRFreeMonitors(Monitors);
    }

    XCloseDisplay(theDisplay);
    exit(0);
}

void
RestoreCursorHandler(int signum)
{
    (void)signum;
    TerminationRequested = 1;
}

Bool
ProcessKeyPress(XKeyEvent *theKeyEvent)
{
    int Length;
    int theKeyBufferMaxLen = AVAIL_KEYBUF;
    char theKeyBuffer[AVAIL_KEYBUF + 1];
    KeySym theKeySym;
    XComposeStatus theComposeStatus;
    Bool ReturnState;

    ReturnState = True;

    Length = XLookupString(theKeyEvent,
                           theKeyBuffer, theKeyBufferMaxLen,
                           &theKeySym, &theComposeStatus);

    if (Length > 0) {
        switch (theKeyBuffer[0]) {
        case 'q':
        case 'Q':
            if (theKeyEvent->state & Mod1Mask) {
                ReturnState = False;
            }
            break;
        default:
            break;
        }
    }

    return ReturnState;
}

Bool
ProcessEvent(void)
{
    XEvent theEvent;
    Bool ContinueState = True;

    while (XPending(theDisplay)) {
        XNextEvent(theDisplay, &theEvent);
        switch (theEvent.type) {
        case Expose:
            if (theEvent.xexpose.count == 0) {
                if (Bed.enabled && theEvent.xexpose.window == BedWindow) {
                    XFillRectangle(theDisplay, BedWindow, Bed.gc, 0, 0,
                            bed_width, bed_height);
                } else if (theEvent.xexpose.window == theWindow) {
                    RedrawNeko();
                }
            }
            break;
        case KeyPress:
            ContinueState = ProcessKeyPress(&theEvent.xkey);
            if (!ContinueState) {
                return ContinueState;
            }
            break;
        case VisibilityNotify:
            if (Neko.raise_window_delay == 0) {
                XRaiseWindow(theDisplay, theWindow);
                Neko.raise_window_delay = DEFAULT_RAISE_WAIT;
            }
            break;
        case ButtonPress:
            if (Bed.enabled && theEvent.xbutton.window == BedWindow &&
                theEvent.xbutton.button == Button1) {
                if (XGrabPointer(theDisplay, BedWindow, False,
                        ButtonReleaseMask | PointerMotionMask,
                        GrabModeAsync, GrabModeAsync, None, None,
                        theEvent.xbutton.time) == GrabSuccess) {
                    Bed.dragging = 1;
                    Bed.drag_offset_x = theEvent.xbutton.x;
                    Bed.drag_offset_y = theEvent.xbutton.y;
                }
            }
            break;
        case MotionNotify:
            if (Bed.enabled && Bed.dragging) {
                int NewX = theEvent.xmotion.x_root - Bed.drag_offset_x;
                int NewY = theEvent.xmotion.y_root - Bed.drag_offset_y;

                if (RectOnMonitor(NewX, NewY, bed_width, bed_height)) {
                    Bed.x = NewX;
                    Bed.y = NewY;
                    XMoveWindow(theDisplay, BedWindow, Bed.x, Bed.y);
                }
            }
            break;
        case ButtonRelease:
            if (Bed.enabled && Bed.dragging && theEvent.xbutton.button == Button1) {
                Bed.dragging = 0;
                XUngrabPointer(theDisplay, theEvent.xbutton.time);
                SaveBedPosition();
            }
            break;
        default:
            break;
        }
    }

    return ContinueState;
}

void
RedrawNeko(void)
{
    XFillRectangle(theDisplay, theWindow, Neko.last_gc,
                   0, 0, BITMAP_WIDTH, BITMAP_HEIGHT);
    XFlush(theDisplay);
}
