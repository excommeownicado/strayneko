#include "strayneko.h"
#include <stdarg.h>

char *ClassName = "Strayneko";
char *ProgramName;

Display *theDisplay;
int theScreen;
unsigned int theDepth;
Window theRoot;
Window theWindow;
Window BedWindow;

GC BedGC;
Pixmap BedPixmap;
Pixmap BedMask;

unsigned int WindowWidth;
unsigned int WindowHeight;

XColor theForegroundColor;
XColor theBackgroundColor;

int Synchronous = False;
volatile sig_atomic_t TerminationRequested = 0;

char *Foreground = NULL;
char *Background = NULL;
long IntervalTime = 0L;
double NekoSpeed = 0.0;
int NoShape = NOTDEFINED;
int ReverseVideo = NOTDEFINED;

Bool DontMapped = True;
int NekoTickCount;
int NekoStateCount;
int NekoState;

XRRMonitorInfo *Monitors = NULL;
int MonitorCount = 0;

int RestrictMonitor = -1;
int TargetX;
int TargetY;
int ForceTargetFlag = 0;
int ForceTargetX = 0;
int ForceTargetY = 0;

int BedX = 500;
int BedY = 500;

int GoingToBed;
Bool UseBed = False;

int DraggingBed = 0;
int DragOffsetX = 0;
int DragOffsetY = 0;

#ifdef ENABLE_DEBUG
int DebugMode = 0;

static void
DebugLog(const char *format, ...)
{
    va_list args;

    if (!DebugMode) {
        return;
    }

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}
#else
#define DebugLog(...) ((void)0)
#endif

int Waiting = 0;
time_t NextMoveTime = 0;

time_t ZoomiesEndTime = 0;
int Zoomies = 0;

int MinWait = 20;
int MaxWait = 150;

int NekoX;
int NekoY;
int NekoMoveDx;
int NekoMoveDy;
int NekoLastX;
int NekoLastY;
GC NekoLastGC;

int RaiseWindowDelay = 0;

double SinPiPer8Times3;
double SinPiPer8;

Pixmap Mati2Xbm, Jare2Xbm, Kaki1Xbm, Kaki2Xbm, Mati3Xbm, Sleep1Xbm, Sleep2Xbm;
Pixmap Mati2Msk, Jare2Msk, Kaki1Msk, Kaki2Msk, Mati3Msk, Sleep1Msk, Sleep2Msk;
Pixmap AwakeXbm, AwakeMsk;
Pixmap Up1Xbm, Up2Xbm, Down1Xbm, Down2Xbm, Left1Xbm, Left2Xbm;
Pixmap Up1Msk, Up2Msk, Down1Msk, Down2Msk, Left1Msk, Left2Msk;
Pixmap Right1Xbm, Right2Xbm, UpLeft1Xbm, UpLeft2Xbm, UpRight1Xbm;
Pixmap Right1Msk, Right2Msk, UpLeft1Msk, UpLeft2Msk, UpRight1Msk;
Pixmap UpRight2Xbm, DownLeft1Xbm, DownLeft2Xbm, DownRight1Xbm, DownRight2Xbm;
Pixmap UpRight2Msk, DownLeft1Msk, DownLeft2Msk, DownRight1Msk, DownRight2Msk;
Pixmap UpTogi1Xbm, UpTogi2Xbm, DownTogi1Xbm, DownTogi2Xbm, LeftTogi1Xbm;
Pixmap UpTogi1Msk, UpTogi2Msk, DownTogi1Msk, DownTogi2Msk, LeftTogi1Msk;
Pixmap LeftTogi2Xbm, RightTogi1Xbm, RightTogi2Xbm;
Pixmap LeftTogi2Msk, RightTogi1Msk, RightTogi2Msk;

GC Mati2GC;
GC Jare2GC, Kaki1GC, Kaki2GC, Mati3GC, Sleep1GC, Sleep2GC;
GC AwakeGC;
GC Up1GC, Up2GC, Down1GC, Down2GC, Left1GC, Left2GC, Right1GC, Right2GC;
GC UpLeft1GC, UpLeft2GC, UpRight1GC, UpRight2GC, DownLeft1GC, DownLeft2GC;
GC DownRight1GC, DownRight2GC;
GC UpTogi1GC, UpTogi2GC, DownTogi1GC, DownTogi2GC, LeftTogi1GC;
GC LeftTogi2GC, RightTogi1GC, RightTogi2GC;

Animation AnimationPattern[][2] = {
    { { &Mati2GC, &Mati2Msk }, { &Mati2GC, &Mati2Msk } },
    { { &Jare2GC, &Jare2Msk }, { &Mati2GC, &Mati2Msk } },
    { { &Kaki1GC, &Kaki1Msk }, { &Kaki2GC, &Kaki2Msk } },
    { { &Mati3GC, &Mati3Msk }, { &Mati3GC, &Mati3Msk } },
    { { &Sleep1GC, &Sleep1Msk }, { &Sleep2GC, &Sleep2Msk } },
    { { &AwakeGC, &AwakeMsk }, { &AwakeGC, &AwakeMsk } },
    { { &Up1GC, &Up1Msk }, { &Up2GC, &Up2Msk } },
    { { &Down1GC, &Down1Msk }, { &Down2GC, &Down2Msk } },
    { { &Left1GC, &Left1Msk }, { &Left2GC, &Left2Msk } },
    { { &Right1GC, &Right1Msk }, { &Right2GC, &Right2Msk } },
    { { &UpLeft1GC, &UpLeft1Msk }, { &UpLeft2GC, &UpLeft2Msk } },
    { { &UpRight1GC, &UpRight1Msk }, { &UpRight2GC, &UpRight2Msk } },
    { { &DownLeft1GC, &DownLeft1Msk }, { &DownLeft2GC, &DownLeft2Msk } },
    { { &DownRight1GC, &DownRight1Msk }, { &DownRight2GC, &DownRight2Msk } },
    { { &UpTogi1GC, &UpTogi1Msk }, { &UpTogi2GC, &UpTogi2Msk } },
    { { &DownTogi1GC, &DownTogi1Msk }, { &DownTogi2GC, &DownTogi2Msk } },
    { { &LeftTogi1GC, &LeftTogi1Msk }, { &LeftTogi2GC, &LeftTogi2Msk } },
    { { &RightTogi1GC, &RightTogi1Msk }, { &RightTogi2GC, &RightTogi2Msk } },
};

static int
FindMonitorFor(int x, int y)
{
    for (int i = 0; i < MonitorCount; i++) {
        if (x >= Monitors[i].x && y >= Monitors[i].y &&
            x < Monitors[i].x + Monitors[i].width &&
            y < Monitors[i].y + Monitors[i].height) {
            return i;
        }
    }
    return -1;
}

static int
GetTargetEdgeTogiState(void)
{
    int monitor;
    int min_x, min_y, max_x, max_y;

    if (MonitorCount <= 0) {
        min_x = 0;
        min_y = 0;
        max_x = WindowWidth > BITMAP_WIDTH ? WindowWidth - BITMAP_WIDTH : 0;
        max_y = WindowHeight > BITMAP_HEIGHT ? WindowHeight - BITMAP_HEIGHT : 0;
    } else {
        monitor = FindMonitorFor(TargetX, TargetY);
        if (monitor < 0) {
            monitor = FindMonitorFor(NekoX + BITMAP_WIDTH / 2, NekoY + BITMAP_HEIGHT / 2);
        }
        if (monitor < 0) {
            monitor = 0;
        }

        min_x = Monitors[monitor].x;
        min_y = Monitors[monitor].y;
        max_x = Monitors[monitor].width > BITMAP_WIDTH ? Monitors[monitor].x + Monitors[monitor].width - BITMAP_WIDTH : Monitors[monitor].x;
        max_y = Monitors[monitor].height > BITMAP_HEIGHT ? Monitors[monitor].y + Monitors[monitor].height - BITMAP_HEIGHT : Monitors[monitor].y;
    }

    if (TargetY == min_y) {
        return NEKO_U_TOGI;
    }
    if (TargetY == max_y) {
        return NEKO_D_TOGI;
    }
    if (TargetX == min_x) {
        return NEKO_L_TOGI;
    }
    if (TargetX == max_x) {
        return NEKO_R_TOGI;
    }
    return -1;
}

static void
ClampTarget(void)
{
    if (MonitorCount <= 0) {
        if (TargetX < 0) {
            DebugLog("ClampTarget: TargetX %d -> 0 (root)\n", TargetX);
            TargetX = 0;
        }
        int max_x = WindowWidth > BITMAP_WIDTH ? WindowWidth - BITMAP_WIDTH : 0;
        if (TargetX > max_x) {
            DebugLog("ClampTarget: TargetX %d -> %d (root)\n", TargetX, max_x);
            TargetX = max_x;
        }
        if (TargetY < 0) {
            DebugLog("ClampTarget: TargetY %d -> 0 (root)\n", TargetY);
            TargetY = 0;
        }
        int max_y = WindowHeight > BITMAP_HEIGHT ? WindowHeight - BITMAP_HEIGHT : 0;
        if (TargetY > max_y) {
            DebugLog("ClampTarget: TargetY %d -> %d (root)\n", TargetY, max_y);
            TargetY = max_y;
        }
        return;
    }

    int monitor = -1;
    if (RestrictMonitor >= 0 && RestrictMonitor < MonitorCount) {
        monitor = RestrictMonitor;
    } else {
        monitor = FindMonitorFor(TargetX, TargetY);
        if (monitor < 0) {
            monitor = FindMonitorFor(NekoX + BITMAP_WIDTH / 2, NekoY + BITMAP_HEIGHT);
        }
        if (monitor < 0) {
            monitor = 0;
        }
    }

    int min_x = Monitors[monitor].x;
    int min_y = Monitors[monitor].y;
    int max_x = Monitors[monitor].width > BITMAP_WIDTH ? Monitors[monitor].x + Monitors[monitor].width - BITMAP_WIDTH : Monitors[monitor].x;
    int max_y = Monitors[monitor].height > BITMAP_HEIGHT ? Monitors[monitor].y + Monitors[monitor].height - BITMAP_HEIGHT : Monitors[monitor].y;

    if (TargetX < min_x) {
        DebugLog("ClampTarget: TargetX %d -> %d (monitor %d)\n", TargetX, min_x, monitor);
        TargetX = min_x;
    } else if (TargetX > max_x) {
        DebugLog("ClampTarget: TargetX %d -> %d (monitor %d)\n", TargetX, max_x, monitor);
        TargetX = max_x;
    }

    if (TargetY < min_y) {
        DebugLog("ClampTarget: TargetY %d -> %d (monitor %d)\n", TargetY, min_y, monitor);
        TargetY = min_y;
    } else if (TargetY > max_y) {
        DebugLog("ClampTarget: TargetY %d -> %d (monitor %d)\n", TargetY, max_y, monitor);
        TargetY = max_y;
    }

    if (!RectOnMonitor(TargetX, TargetY, BITMAP_WIDTH, BITMAP_HEIGHT)) {
        DebugLog("ClampTarget: Target (%d,%d) still invalid for monitor %d, resetting to (%d,%d)\n",
                 TargetX, TargetY, monitor, min_x, min_y);
        TargetX = min_x;
        TargetY = min_y;
    }
}

void
Interval(void)
{
    pause();
    if (RaiseWindowDelay > 0) {
        RaiseWindowDelay--;
    }
}

void
TickCount(void)
{
    if (++NekoTickCount >= MAX_TICK) {
        NekoTickCount = 0;
    }

    if (NekoTickCount % 2 == 0) {
        if (NekoStateCount < MAX_TICK) {
            NekoStateCount++;
        }
    }
}

void
SetNekoState(int SetValue)
{
    NekoTickCount = 0;
    NekoStateCount = 0;
    NekoState = SetValue;
}

void
DrawNeko(int x, int y, Animation DrawAnime)
{
    register GC DrawGC = *(DrawAnime.TickGCPtr);
    register Pixmap DrawMask = *(DrawAnime.TickMaskPtr);

    if ((x != NekoLastX) || (y != NekoLastY) || (DrawGC != NekoLastGC)) {
        XWindowChanges theChanges;

        theChanges.x = x;
        theChanges.y = y;
        XConfigureWindow(theDisplay, theWindow, CWX | CWY, &theChanges);
#ifdef SHAPE
        if (NoShape == False) {
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
    NekoLastX = x;
    NekoLastY = y;
    NekoLastGC = DrawGC;
}

void
NekoDirection(void)
{
    int NewState;
    double LargeX, LargeY;
    double Length;
    double SinTheta;

    if (NekoMoveDx == 0 && NekoMoveDy == 0) {
        NewState = NEKO_STOP;
    } else {
        LargeX = (double)NekoMoveDx;
        LargeY = (double)(-NekoMoveDy);
        Length = sqrt(LargeX * LargeX + LargeY * LargeY);
        SinTheta = LargeY / Length;

        if (NekoMoveDx > 0) {
            if (SinTheta > SinPiPer8Times3) {
                NewState = NEKO_U_MOVE;
            } else if ((SinTheta <= SinPiPer8Times3) && (SinTheta > SinPiPer8)) {
                NewState = NEKO_UR_MOVE;
            } else if ((SinTheta <= SinPiPer8) && (SinTheta > -(SinPiPer8))) {
                NewState = NEKO_R_MOVE;
            } else if ((SinTheta <= -(SinPiPer8)) && (SinTheta > -(SinPiPer8Times3))) {
                NewState = NEKO_DR_MOVE;
            } else {
                NewState = NEKO_D_MOVE;
            }
        } else {
            if (SinTheta > SinPiPer8Times3) {
                NewState = NEKO_U_MOVE;
            } else if ((SinTheta <= SinPiPer8Times3) && (SinTheta > SinPiPer8)) {
                NewState = NEKO_UL_MOVE;
            } else if ((SinTheta <= SinPiPer8) && (SinTheta > -(SinPiPer8))) {
                NewState = NEKO_L_MOVE;
            } else if ((SinTheta <= -(SinPiPer8)) && (SinTheta > -(SinPiPer8Times3))) {
                NewState = NEKO_DL_MOVE;
            } else {
                NewState = NEKO_D_MOVE;
            }
        }
    }

    if (NekoState != NewState) {
        SetNekoState(NewState);
    }
}

Bool
IsWindowOver(void)
{
    Bool ReturnValue = False;

    if (NekoY <= 0) {
        NekoY = 0;
        ReturnValue = True;
    } else if (NekoY >= WindowHeight - BITMAP_HEIGHT) {
        NekoY = WindowHeight - BITMAP_HEIGHT;
        ReturnValue = True;
    }
    if (NekoX <= 0) {
        NekoX = 0;
        ReturnValue = True;
    } else if (NekoX >= WindowWidth - BITMAP_WIDTH) {
        NekoX = WindowWidth - BITMAP_WIDTH;
        ReturnValue = True;
    }

    return ReturnValue;
}

Bool
IsNekoDontMove(void)
{
    return NekoX == NekoLastX && NekoY == NekoLastY;
}

Bool
IsNekoMoveStart(void)
{
    return NekoMoveDx != 0 || NekoMoveDy != 0;
}

void
PickRandomTarget(void)
{
    int m = -1;

    if (MonitorCount <= 0) {
        int width = WindowWidth > BITMAP_WIDTH ? WindowWidth - BITMAP_WIDTH : 0;
        int height = WindowHeight > BITMAP_HEIGHT ? WindowHeight - BITMAP_HEIGHT : 0;

        TargetX = rand() % (width + 1);
        TargetY = rand() % (height + 1);
        DebugLog("PickRandomTarget: root target=(%d,%d) width=%d height=%d\n",
                 TargetX, TargetY, width, height);
        return;
    }

    if (RestrictMonitor >= 0 && RestrictMonitor < MonitorCount) {
        m = RestrictMonitor;
    } else {
        m = rand() % MonitorCount;
    }

    int width = Monitors[m].width > BITMAP_WIDTH ? Monitors[m].width - BITMAP_WIDTH : 0;
    int height = Monitors[m].height > BITMAP_HEIGHT ? Monitors[m].height - BITMAP_HEIGHT : 0;

    TargetX = Monitors[m].x + rand() % (width + 1);
    TargetY = Monitors[m].y + rand() % (height + 1);
    DebugLog("PickRandomTarget: monitor=%d target=(%d,%d) width=%d height=%d\n",
             m, TargetX, TargetY, width, height);
}

int
RectOnMonitor(int x, int y, int w, int h)
{
    int i;

    if (MonitorCount <= 0) {
        return x >= 0 && y >= 0 && x + w <= (int)WindowWidth && y + h <= (int)WindowHeight;
    }

    if (RestrictMonitor >= 0 && RestrictMonitor < MonitorCount) {
        i = RestrictMonitor;
        return x >= Monitors[i].x && y >= Monitors[i].y &&
            x + w <= Monitors[i].x + Monitors[i].width &&
            y + h <= Monitors[i].y + Monitors[i].height;
    }

    for (i = 0; i < MonitorCount; i++) {
        if (x >= Monitors[i].x && y >= Monitors[i].y &&
            x + w <= Monitors[i].x + Monitors[i].width &&
            y + h <= Monitors[i].y + Monitors[i].height) {
            return 1;
        }
    }

    return 0;
}

void
PickNearbyTarget(int radius)
{
    int x, y;

    for (int tries = 0; tries < 100; tries++) {
        x = NekoX + (rand() % (radius * 2 + 1) - radius);
        y = NekoY + (rand() % (radius * 2 + 1) - radius);

        if (RectOnMonitor(x, y, BITMAP_WIDTH, BITMAP_HEIGHT)) {
            TargetX = x;
            TargetY = y;
            DebugLog("PickNearbyTarget: target=(%d,%d) tries=%d\n", TargetX, TargetY, tries + 1);
            return;
        }
    }

    PickRandomTarget();
    DebugLog("PickNearbyTarget: fallback target=(%d,%d)\n", TargetX, TargetY);
}

void
CalcDxDy(void)
{
    double LargeX, LargeY;
    double DoubleLength, Length;

    if (DebugMode) {
        DebugLog("CalcDxDy: start Target=(%d,%d) Neko=(%d,%d) Waiting=%d Zoomies=%d\n",
                 TargetX, TargetY, NekoX, NekoY, Waiting, Zoomies);
    }

    if (Zoomies && time(NULL) > ZoomiesEndTime) {
        Zoomies = 0;
    }

    if (Waiting) {
            if (time(NULL) < NextMoveTime) {
                NekoMoveDx = 0;
                NekoMoveDy = 0;
                return;
            }

            Waiting = 0;
            GoingToBed = 0;

            if (Zoomies) {
                PickRandomTarget();
            } else {
                int r = rand() % 100;

            if (UseBed && r < 15) {
                /* BedX/BedY are stored as top-left; target should be top-left too. */
                TargetX = BedX;
                TargetY = BedY;
                GoingToBed = 1;
            } else if (r < (UseBed ? 55 : 40)) {
                PickNearbyTarget(300);
            } else if (r < (UseBed ? 85 : 70)) {
                PickRandomTarget();
            } else {
                TargetX = NekoX;
                TargetY = NekoY;
            }
        }

            if (!Zoomies && rand() % 100 < 5) {
            Zoomies = 1;
            ZoomiesEndTime = time(NULL) + 50;
        }

            DebugLog("CalcDxDy: selected target=(%d,%d) goingToBed=%d Zoomies=%d Neko=(%d,%d)\n",
                     TargetX, TargetY, GoingToBed, Zoomies, NekoX, NekoY);
        ClampTarget();
    }

    int ReachDistance = 2;

    /* Consider Neko reached the target when its top-left aligns (within tolerance).
       This handles edge cases where IsWindowOver clamps Neko positions at screen edges. */
    if (!Waiting &&
        abs(TargetX - NekoX) <= ReachDistance &&
        abs(TargetY - NekoY) <= ReachDistance) {
        int delay;

        Waiting = 1;
        delay = (int)(MinWait + rand() % (MaxWait - MinWait + 1));

        if (GoingToBed) {
            delay *= 2;
        }
        if (Zoomies) {
            delay = 1 + rand() % 3;
        }

        NextMoveTime = time(NULL) + delay;
        NekoMoveDx = 0;
        NekoMoveDy = 0;
        if (NekoState != NEKO_STOP) {
            SetNekoState(NEKO_STOP);
        }
        return;
    }

    /* Use top-left coordinates for both Target and Neko so edges align correctly. */
    LargeX = (double)(TargetX - NekoX);
    LargeY = (double)(TargetY - NekoY);

    DoubleLength = LargeX * LargeX + LargeY * LargeY;
    if (DoubleLength != 0.0) {
        Length = sqrt(DoubleLength);
        if (Length <= NekoSpeed) {
            NekoMoveDx = (int)LargeX;
            NekoMoveDy = (int)LargeY;
        } else {
            NekoMoveDx = (int)((NekoSpeed * LargeX) / Length);
            NekoMoveDy = (int)((NekoSpeed * LargeY) / Length);
        }

        if (NekoMoveDx == 0 && NekoMoveDy == 0) {
            NekoMoveDx = LargeX > 0 ? 1 : (LargeX < 0 ? -1 : 0);
            NekoMoveDy = LargeY > 0 ? 1 : (LargeY < 0 ? -1 : 0);
            if (NekoMoveDx == 0 && NekoMoveDy == 0) {
                NekoMoveDx = 1;
            }
            DebugLog("CalcDxDy: forced minimal move=(%d,%d) for vector=(%.2f,%.2f)\n",
                     NekoMoveDx, NekoMoveDy, LargeX, LargeY);
        }
    } else {
        NekoMoveDx = 0;
        NekoMoveDy = 0;
    }

    /* If movement vector is exactly zero (e.g., due to centering offsets),
       treat as having reached the target so Neko doesn't get stuck. */
    if (!Waiting && NekoMoveDx == 0 && NekoMoveDy == 0) {
        int delay;

        Waiting = 1;
        delay = (int)(MinWait + rand() % (MaxWait - MinWait + 1));

        if (GoingToBed) {
            delay *= 2;
        }
        if (Zoomies) {
            delay = 1 + rand() % 3;
        }

        NextMoveTime = time(NULL) + delay;
        if (NekoState != NEKO_STOP) {
            SetNekoState(NEKO_STOP);
        }
        return;
    }

    if (DebugMode) {
        DebugLog("CalcDxDy: vector=(%.2f,%.2f) speed=%.2f move=(%d,%d)\n",
                 LargeX, LargeY, NekoSpeed, NekoMoveDx, NekoMoveDy);
    }
}

void
NekoThinkDraw(void)
{
    CalcDxDy();

    if (NekoState != NEKO_SLEEP) {
        DrawNeko(NekoX, NekoY,
                AnimationPattern[NekoState][NekoTickCount & 0x1]);
    } else {
        DrawNeko(NekoX, NekoY,
                AnimationPattern[NekoState][(NekoTickCount >> 2) & 0x1]);
    }

    TickCount();

    switch (NekoState) {
    case NEKO_STOP:
        if (IsNekoMoveStart()) {
            SetNekoState(NEKO_AWAKE);
            break;
        }
        if (NekoStateCount < NEKO_STOP_TIME) {
            break;
        }
        {
            int edgeState = GetTargetEdgeTogiState();
            if (edgeState >= 0) {
                DebugLog("NEKO_STOP: target at monitor edge, selecting state=%d\n", edgeState);
                SetNekoState(edgeState);
                break;
            }
        }
        if (NekoMoveDx < 0 && NekoX <= 0) {
            SetNekoState(NEKO_L_TOGI);
        } else if (NekoMoveDx > 0 && NekoX >= WindowWidth - BITMAP_WIDTH) {
            SetNekoState(NEKO_R_TOGI);
        } else if (NekoMoveDy < 0 && NekoY <= 0) {
            SetNekoState(NEKO_U_TOGI);
        } else if (NekoMoveDy > 0 && NekoY >= WindowHeight - BITMAP_HEIGHT) {
            SetNekoState(NEKO_D_TOGI);
        } else {
            SetNekoState(NEKO_JARE);
        }
        break;
    case NEKO_JARE:
        if (IsNekoMoveStart()) {
            SetNekoState(NEKO_AWAKE);
            break;
        }
        if (NekoStateCount < NEKO_JARE_TIME) {
            break;
        }
        SetNekoState(NEKO_KAKI);
        break;
    case NEKO_KAKI:
        if (IsNekoMoveStart()) {
            SetNekoState(NEKO_AWAKE);
            break;
        }
        if (NekoStateCount < NEKO_KAKI_TIME) {
            break;
        }
        SetNekoState(NEKO_AKUBI);
        break;
    case NEKO_AKUBI:
        if (IsNekoMoveStart()) {
            SetNekoState(NEKO_AWAKE);
            break;
        }
        if (NekoStateCount < NEKO_AKUBI_TIME) {
            break;
        }
        SetNekoState(NEKO_SLEEP);
        break;
    case NEKO_SLEEP:
        if (IsNekoMoveStart()) {
            SetNekoState(NEKO_AWAKE);
            break;
        }
        break;
    case NEKO_AWAKE:
        if (NekoStateCount < NEKO_AWAKE_TIME) {
            break;
        }
        NekoDirection();
        break;
    case NEKO_U_MOVE:
    case NEKO_D_MOVE:
    case NEKO_L_MOVE:
    case NEKO_R_MOVE:
    case NEKO_UL_MOVE:
    case NEKO_UR_MOVE:
    case NEKO_DL_MOVE:
    case NEKO_DR_MOVE:
        NekoX += NekoMoveDx;
        NekoY += NekoMoveDy;
        NekoDirection();
        if (IsWindowOver()) {
            if (IsNekoDontMove()) {
                SetNekoState(NEKO_STOP);
            }
        }
        break;
    case NEKO_U_TOGI:
    case NEKO_D_TOGI:
    case NEKO_L_TOGI:
    case NEKO_R_TOGI:
        if (IsNekoMoveStart()) {
            SetNekoState(NEKO_AWAKE);
            break;
        }
        if (NekoStateCount < NEKO_TOGI_TIME) {
            break;
        }
        SetNekoState(NEKO_KAKI);
        break;
    default:
        SetNekoState(NEKO_STOP);
        break;
    }

    Interval();
}

void
ProcessNeko(void)
{
    struct itimerval Value;

    PickRandomTarget();

    if (ForceTargetFlag) {
        /* place Neko near center and force a target at the edge for testing */
        NekoX = (int)WindowWidth / 2;
        NekoY = (int)WindowHeight / 2;
        TargetX = ForceTargetX;
        TargetY = ForceTargetY;
        /* ensure forced target is clamped to a valid location */
        ClampTarget();
        NekoLastX = NekoX;
        NekoLastY = NekoY;
        Waiting = 0; /* start moving immediately toward forced target */
        DebugLog("ProcessNeko: Forced target=(%d,%d) start=(%d,%d)\n", TargetX, TargetY, NekoX, NekoY);
    } else {
        NekoX = TargetX;
        NekoY = TargetY;
        NekoLastX = NekoX;
        NekoLastY = NekoY;
        Waiting = 1;
        NextMoveTime = time(NULL);
        SetNekoState(NEKO_STOP);
    }

    timerclear(&Value.it_interval);
    timerclear(&Value.it_value);

    Value.it_interval.tv_sec = IntervalTime / 1000000L;
    Value.it_interval.tv_usec = IntervalTime % 1000000L;
    Value.it_value.tv_sec = Value.it_interval.tv_sec;
    Value.it_value.tv_usec = Value.it_interval.tv_usec;

    if (setitimer(ITIMER_REAL, &Value, 0) != 0) {
        perror("setitimer");
        return;
    }

    do {
        NekoThinkDraw();
    } while (!TerminationRequested && ProcessEvent());
}
