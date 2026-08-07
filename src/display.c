#include "strayneko.h"

static void
InitSpriteData(void)
{
    Sprites[SPRITE_MATI2].bits = mati2_bits;
    Sprites[SPRITE_MATI2].mask_bits = mati2_mask_bits;

    Sprites[SPRITE_JARE2].bits = jare2_bits;
    Sprites[SPRITE_JARE2].mask_bits = jare2_mask_bits;

    Sprites[SPRITE_KAKI1].bits = kaki1_bits;
    Sprites[SPRITE_KAKI1].mask_bits = kaki1_mask_bits;

    Sprites[SPRITE_KAKI2].bits = kaki2_bits;
    Sprites[SPRITE_KAKI2].mask_bits = kaki2_mask_bits;

    Sprites[SPRITE_MATI3].bits = mati3_bits;
    Sprites[SPRITE_MATI3].mask_bits = mati3_mask_bits;

    Sprites[SPRITE_SLEEP1].bits = sleep1_bits;
    Sprites[SPRITE_SLEEP1].mask_bits = sleep1_mask_bits;

    Sprites[SPRITE_SLEEP2].bits = sleep2_bits;
    Sprites[SPRITE_SLEEP2].mask_bits = sleep2_mask_bits;

    Sprites[SPRITE_AWAKE].bits = awake_bits;
    Sprites[SPRITE_AWAKE].mask_bits = awake_mask_bits;

    Sprites[SPRITE_UP1].bits = up1_bits;
    Sprites[SPRITE_UP1].mask_bits = up1_mask_bits;

    Sprites[SPRITE_UP2].bits = up2_bits;
    Sprites[SPRITE_UP2].mask_bits = up2_mask_bits;

    Sprites[SPRITE_DOWN1].bits = down1_bits;
    Sprites[SPRITE_DOWN1].mask_bits = down1_mask_bits;

    Sprites[SPRITE_DOWN2].bits = down2_bits;
    Sprites[SPRITE_DOWN2].mask_bits = down2_mask_bits;

    Sprites[SPRITE_LEFT1].bits = left1_bits;
    Sprites[SPRITE_LEFT1].mask_bits = left1_mask_bits;

    Sprites[SPRITE_LEFT2].bits = left2_bits;
    Sprites[SPRITE_LEFT2].mask_bits = left2_mask_bits;

    Sprites[SPRITE_RIGHT1].bits = right1_bits;
    Sprites[SPRITE_RIGHT1].mask_bits = right1_mask_bits;

    Sprites[SPRITE_RIGHT2].bits = right2_bits;
    Sprites[SPRITE_RIGHT2].mask_bits = right2_mask_bits;

    Sprites[SPRITE_UPLEFT1].bits = upleft1_bits;
    Sprites[SPRITE_UPLEFT1].mask_bits = upleft1_mask_bits;

    Sprites[SPRITE_UPLEFT2].bits = upleft2_bits;
    Sprites[SPRITE_UPLEFT2].mask_bits = upleft2_mask_bits;

    Sprites[SPRITE_UPRIGHT1].bits = upright1_bits;
    Sprites[SPRITE_UPRIGHT1].mask_bits = upright1_mask_bits;

    Sprites[SPRITE_UPRIGHT2].bits = upright2_bits;
    Sprites[SPRITE_UPRIGHT2].mask_bits = upright2_mask_bits;

    Sprites[SPRITE_DOWNLEFT1].bits = dwleft1_bits;
    Sprites[SPRITE_DOWNLEFT1].mask_bits = dwleft1_mask_bits;

    Sprites[SPRITE_DOWNLEFT2].bits = dwleft2_bits;
    Sprites[SPRITE_DOWNLEFT2].mask_bits = dwleft2_mask_bits;

    Sprites[SPRITE_DOWNRIGHT1].bits = dwright1_bits;
    Sprites[SPRITE_DOWNRIGHT1].mask_bits = dwright1_mask_bits;

    Sprites[SPRITE_DOWNRIGHT2].bits = dwright2_bits;
    Sprites[SPRITE_DOWNRIGHT2].mask_bits = dwright2_mask_bits;

    Sprites[SPRITE_UPTOGI1].bits = utogi1_bits;
    Sprites[SPRITE_UPTOGI1].mask_bits = utogi1_mask_bits;

    Sprites[SPRITE_UPTOGI2].bits = utogi2_bits;
    Sprites[SPRITE_UPTOGI2].mask_bits = utogi2_mask_bits;

    Sprites[SPRITE_DOWNTOGI1].bits = dtogi1_bits;
    Sprites[SPRITE_DOWNTOGI1].mask_bits = dtogi1_mask_bits;

    Sprites[SPRITE_DOWNTOGI2].bits = dtogi2_bits;
    Sprites[SPRITE_DOWNTOGI2].mask_bits = dtogi2_mask_bits;

    Sprites[SPRITE_LEFTTOGI1].bits = ltogi1_bits;
    Sprites[SPRITE_LEFTTOGI1].mask_bits = ltogi1_mask_bits;

    Sprites[SPRITE_LEFTTOGI2].bits = ltogi2_bits;
    Sprites[SPRITE_LEFTTOGI2].mask_bits = ltogi2_mask_bits;

    Sprites[SPRITE_RIGHTTOGI1].bits = rtogi1_bits;
    Sprites[SPRITE_RIGHTTOGI1].mask_bits = rtogi1_mask_bits;

    Sprites[SPRITE_RIGHTTOGI2].bits = rtogi2_bits;
    Sprites[SPRITE_RIGHTTOGI2].mask_bits = rtogi2_mask_bits;
}

static void
CreateBedWindow(void)
{
    XSetWindowAttributes theWindowAttributes;
    unsigned long theWindowMask;

    theWindowAttributes.background_pixel = theBackgroundColor.pixel;
    theWindowAttributes.override_redirect = True;

    theWindowMask = CWBackPixel | CWOverrideRedirect;

    BedWindow = XCreateWindow(theDisplay, theRoot, BedX, BedY,
            bed_width, bed_height,
            0, theDepth, InputOutput, CopyFromParent,
            theWindowMask, &theWindowAttributes);
    BedPixmap = XCreateBitmapFromData(theDisplay, theRoot, bed_bits,
            bed_width, bed_height);
    BedMask = XCreateBitmapFromData(theDisplay, theRoot, bed_mask_bits,
            bed_mask_width, bed_mask_height);
#ifdef SHAPE
    if (!NoShape) {
        XShapeCombineMask(theDisplay, BedWindow, ShapeBounding, 0, 0,
                BedMask, ShapeSet);
    }
#endif
    BedGC = XCreateGC(theDisplay, theRoot, 0, NULL);
    XSetForeground(theDisplay, BedGC, theForegroundColor.pixel);
    XSetBackground(theDisplay, BedGC, theBackgroundColor.pixel);
    XSetStipple(theDisplay, BedGC, BedPixmap);
    XSetFillStyle(theDisplay, BedGC, FillOpaqueStippled);
    XSelectInput(theDisplay, BedWindow,
            ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    XMapWindow(theDisplay, BedWindow);
    XFillRectangle(theDisplay, BedWindow, BedGC, 0, 0, bed_width, bed_height);
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
        Sprite *sprite = &Sprites[i];

        sprite->pixmap = XCreatePixmapFromBitmapData(
            theDisplay,
            theRoot,
            (char *)sprite->bits,
            BITMAP_WIDTH,
            BITMAP_HEIGHT,
            theForegroundColor.pixel,
            theBackgroundColor.pixel,
            DefaultDepth(theDisplay, theScreen));

        theGCValues.tile = sprite->pixmap;

        sprite->mask = XCreateBitmapFromData(
            theDisplay,
            theRoot,
            (char *)sprite->mask_bits,
            BITMAP_WIDTH,
            BITMAP_HEIGHT);

        sprite->gc = XCreateGC(
            theDisplay,
            theWindow,
            GCFunction | GCForeground | GCBackground | GCTile |
            GCTileStipXOrigin | GCTileStipYOrigin | GCFillStyle,
            &theGCValues);
    }
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

    if (RestrictMonitor >= MonitorCount) {
        fprintf(stderr, "%s: monitor %d is unavailable.\n",
                ProgramName, RestrictMonitor);
        exit(1);
    }

    GetResources();

    if (Synchronous == True) {
        fprintf(stderr, "Synchronizing.\n");
        XSynchronize(theDisplay, True);
    }

#ifdef SHAPE
    if (!NoShape && XShapeQueryExtension(theDisplay,
                                         &event_base, &error_base) == False) {
        fprintf(stderr, "Display does not support shape extension.\n");
        NoShape = True;
    }
#endif

    theScreen = DefaultScreen(theDisplay);
    theDepth = DefaultDepth(theDisplay, theScreen);

    theRoot = RootWindow(theDisplay, theScreen);

    XGetGeometry(theDisplay, theRoot, &theTempRoot,
                 &WindowPointX, &WindowPointY,
                 &WindowWidth, &WindowHeight,
                 &BorderWidth, &theDepth);

    if (UseBed) {
        if (!LoadBedPosition() || !RectOnMonitor(BedX, BedY, bed_width, bed_height)) {
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
    InitSpriteData();
    InitBitmapAndGCs();

    XSelectInput(theDisplay, theWindow,
                 ExposureMask | VisibilityChangeMask | KeyPressMask);

    if (UseBed) {
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
        XFreePixmap(theDisplay, Sprites[i].pixmap);
        XFreePixmap(theDisplay, Sprites[i].mask);
        XFreeGC(theDisplay, Sprites[i].gc);
    }

    if (UseBed) {
        XFreePixmap(theDisplay, BedPixmap);
        XFreePixmap(theDisplay, BedMask);
        XFreeGC(theDisplay, BedGC);
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
                if (UseBed && theEvent.xexpose.window == BedWindow) {
                    XFillRectangle(theDisplay, BedWindow, BedGC, 0, 0,
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
            if (RaiseWindowDelay == 0) {
                XRaiseWindow(theDisplay, theWindow);
                RaiseWindowDelay = DEFAULT_RAISE_WAIT;
            }
            break;
        case ButtonPress:
            if (UseBed && theEvent.xbutton.window == BedWindow &&
                theEvent.xbutton.button == Button1) {
                if (XGrabPointer(theDisplay, BedWindow, False,
                        ButtonReleaseMask | PointerMotionMask,
                        GrabModeAsync, GrabModeAsync, None, None,
                        theEvent.xbutton.time) == GrabSuccess) {
                    DraggingBed = 1;
                    DragOffsetX = theEvent.xbutton.x;
                    DragOffsetY = theEvent.xbutton.y;
                }
            }
            break;
        case MotionNotify:
            if (UseBed && DraggingBed) {
                int NewX = theEvent.xmotion.x_root - DragOffsetX;
                int NewY = theEvent.xmotion.y_root - DragOffsetY;

                if (RectOnMonitor(NewX, NewY, bed_width, bed_height)) {
                    BedX = NewX;
                    BedY = NewY;
                    XMoveWindow(theDisplay, BedWindow, BedX, BedY);
                }
            }
            break;
        case ButtonRelease:
            if (UseBed && DraggingBed && theEvent.xbutton.button == Button1) {
                DraggingBed = 0;
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
    XFillRectangle(theDisplay, theWindow, NekoLastGC,
                   0, 0, BITMAP_WIDTH, BITMAP_HEIGHT);
    XFlush(theDisplay);
}
