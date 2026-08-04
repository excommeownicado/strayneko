#include "oneko.h"

static BitmapGCData BitmapGCDataTable[] = {
    { &Mati2GC, &Mati2Xbm, mati2_bits, &Mati2Msk, mati2_mask_bits },
    { &Jare2GC, &Jare2Xbm, jare2_bits, &Jare2Msk, jare2_mask_bits },
    { &Kaki1GC, &Kaki1Xbm, kaki1_bits, &Kaki1Msk, kaki1_mask_bits },
    { &Kaki2GC, &Kaki2Xbm, kaki2_bits, &Kaki2Msk, kaki2_mask_bits },
    { &Mati3GC, &Mati3Xbm, mati3_bits, &Mati3Msk, mati3_mask_bits },
    { &Sleep1GC, &Sleep1Xbm, sleep1_bits, &Sleep1Msk, sleep1_mask_bits },
    { &Sleep2GC, &Sleep2Xbm, sleep2_bits, &Sleep2Msk, sleep2_mask_bits },
    { &AwakeGC, &AwakeXbm, awake_bits, &AwakeMsk, awake_mask_bits },
    { &Up1GC, &Up1Xbm, up1_bits, &Up1Msk, up1_mask_bits },
    { &Up2GC, &Up2Xbm, up2_bits, &Up2Msk, up2_mask_bits },
    { &Down1GC, &Down1Xbm, down1_bits, &Down1Msk, down1_mask_bits },
    { &Down2GC, &Down2Xbm, down2_bits, &Down2Msk, down2_mask_bits },
    { &Left1GC, &Left1Xbm, left1_bits, &Left1Msk, left1_mask_bits },
    { &Left2GC, &Left2Xbm, left2_bits, &Left2Msk, left2_mask_bits },
    { &Right1GC, &Right1Xbm, right1_bits, &Right1Msk, right1_mask_bits },
    { &Right2GC, &Right2Xbm, right2_bits, &Right2Msk, right2_mask_bits },
    { &UpLeft1GC, &UpLeft1Xbm, upleft1_bits, &UpLeft1Msk, upleft1_mask_bits },
    { &UpLeft2GC, &UpLeft2Xbm, upleft2_bits, &UpLeft2Msk, upleft2_mask_bits },
    { &UpRight1GC, &UpRight1Xbm, upright1_bits, &UpRight1Msk, upright1_mask_bits },
    { &UpRight2GC, &UpRight2Xbm, upright2_bits, &UpRight2Msk, upright2_mask_bits },
    { &DownLeft1GC, &DownLeft1Xbm, dwleft1_bits, &DownLeft1Msk, dwleft1_mask_bits },
    { &DownLeft2GC, &DownLeft2Xbm, dwleft2_bits, &DownLeft2Msk, dwleft2_mask_bits },
    { &DownRight1GC, &DownRight1Xbm, dwright1_bits, &DownRight1Msk, dwright1_mask_bits },
    { &DownRight2GC, &DownRight2Xbm, dwright2_bits, &DownRight2Msk, dwright2_mask_bits },
    { &UpTogi1GC, &UpTogi1Xbm, utogi1_bits, &UpTogi1Msk, utogi1_mask_bits },
    { &UpTogi2GC, &UpTogi2Xbm, utogi2_bits, &UpTogi2Msk, utogi2_mask_bits },
    { &DownTogi1GC, &DownTogi1Xbm, dtogi1_bits, &DownTogi1Msk, dtogi1_mask_bits },
    { &DownTogi2GC, &DownTogi2Xbm, dtogi2_bits, &DownTogi2Msk, dtogi2_mask_bits },
    { &LeftTogi1GC, &LeftTogi1Xbm, ltogi1_bits, &LeftTogi1Msk, ltogi1_mask_bits },
    { &LeftTogi2GC, &LeftTogi2Xbm, ltogi2_bits, &LeftTogi2Msk, ltogi2_mask_bits },
    { &RightTogi1GC, &RightTogi1Xbm, rtogi1_bits, &RightTogi1Msk, rtogi1_mask_bits },
    { &RightTogi2GC, &RightTogi2Xbm, rtogi2_bits, &RightTogi2Msk, rtogi2_mask_bits },
    { NULL, NULL, NULL, NULL, NULL }
};

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
    BitmapGCData *BitmapGCDataTablePtr;
    XGCValues theGCValues;

    theGCValues.function = GXcopy;
    theGCValues.foreground = theForegroundColor.pixel;
    theGCValues.background = theBackgroundColor.pixel;
    theGCValues.fill_style = FillTiled;
    theGCValues.ts_x_origin = 0;
    theGCValues.ts_y_origin = 0;

    for (BitmapGCDataTablePtr = BitmapGCDataTable;
         BitmapGCDataTablePtr->GCCreatePtr != NULL;
         BitmapGCDataTablePtr++) {

        *(BitmapGCDataTablePtr->BitmapCreatePtr) = XCreatePixmapFromBitmapData(
            theDisplay, theRoot,
            BitmapGCDataTablePtr->PixelPattern,
            BITMAP_WIDTH, BITMAP_HEIGHT,
            theForegroundColor.pixel,
            theBackgroundColor.pixel,
            DefaultDepth(theDisplay, theScreen));

        theGCValues.tile = *(BitmapGCDataTablePtr->BitmapCreatePtr);
        *(BitmapGCDataTablePtr->BitmapMasksPtr) = XCreateBitmapFromData(
            theDisplay, theRoot,
            BitmapGCDataTablePtr->MaskPattern,
            BITMAP_WIDTH, BITMAP_HEIGHT);

        *(BitmapGCDataTablePtr->GCCreatePtr) = XCreateGC(theDisplay, theWindow,
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
    BitmapGCData *BitmapGCDataTablePtr;

    theWindowAttributes.cursor = None;
    XChangeWindowAttributes(theDisplay, theRoot, CWCursor,
                             &theWindowAttributes);

    for (BitmapGCDataTablePtr = BitmapGCDataTable;
         BitmapGCDataTablePtr->GCCreatePtr != NULL;
         BitmapGCDataTablePtr++) {
        XFreePixmap(theDisplay, *(BitmapGCDataTablePtr->BitmapCreatePtr));
        XFreePixmap(theDisplay, *(BitmapGCDataTablePtr->BitmapMasksPtr));
        XFreeGC(theDisplay, *(BitmapGCDataTablePtr->GCCreatePtr));
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
