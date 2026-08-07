#include "strayneko.h"
#include <stdarg.h>

char *ClassName = "Strayneko";
char *ProgramName;

NekoData Neko = {
    .x = 0,
    .y = 0,
    .target_x = 0,
    .target_y = 0
};

BedData Bed = {
    .x = 500,
    .y = 500,
    .enabled = False
};

ConfigData Config = {
    .foreground = NULL,
    .background = NULL,
    .interval_time = 0L,
    .speed = 0.0,
    .no_shape = NOTDEFINED,
    .reverse_video = NOTDEFINED,
    .min_wait = 20,
    .max_wait = 150,
    .restrict_monitor = -1
};

Display *theDisplay;
int theScreen;
unsigned int theDepth;
Window theRoot;
Window theWindow;
Window BedWindow;

unsigned int WindowWidth;
unsigned int WindowHeight;

XColor theForegroundColor;
XColor theBackgroundColor;

int Synchronous = False;
volatile sig_atomic_t TerminationRequested = 0;

Bool DontMapped = True;

XRRMonitorInfo *Monitors = NULL;
int MonitorCount = 0;

int ForceTargetFlag = 0;
int ForceTargetX = 0;
int ForceTargetY = 0;

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

double SinPiPer8Times3;
double SinPiPer8;

static int
GetTargetEdgeTogiState(void)
{
    int monitor;
    MonitorBounds bounds;

    if (MonitorCount <= 0) {
        bounds.min_x = 0;
        bounds.min_y = 0;
        bounds.max_x = WindowWidth > BITMAP_WIDTH
            ? WindowWidth - BITMAP_WIDTH
            : 0;
        bounds.max_y = WindowHeight > BITMAP_HEIGHT
            ? WindowHeight - BITMAP_HEIGHT
            : 0;
    } else {
        monitor = FindMonitorFor(Neko.target_x, Neko.target_y);

        if (monitor < 0) {
            monitor = FindMonitorFor(
                Neko.x + BITMAP_WIDTH / 2,
                Neko.y + BITMAP_HEIGHT / 2
            );
        }

        if (monitor < 0) {
            monitor = 0;
        }

        bounds = GetMonitorBounds(monitor);
    }

    if (Neko.target_y == bounds.min_y) {
        return NEKO_U_TOGI;
    }

    if (Neko.target_y == bounds.max_y) {
        return NEKO_D_TOGI;
    }

    if (Neko.target_x == bounds.min_x) {
        return NEKO_L_TOGI;
    }

    if (Neko.target_x == bounds.max_x) {
        return NEKO_R_TOGI;
    }

    return -1;
}

static void
ClampTarget(void)
{
    if (MonitorCount <= 0) {
        if (Neko.target_x < 0) {
            DebugLog("ClampTarget: Neko.target_x %d -> 0 (root)\n", Neko.target_x);
            Neko.target_x = 0;
        }
        int max_x = WindowWidth > BITMAP_WIDTH ? WindowWidth - BITMAP_WIDTH : 0;
        if (Neko.target_x > max_x) {
            DebugLog("ClampTarget: Neko.target_x %d -> %d (root)\n", Neko.target_x, max_x);
            Neko.target_x = max_x;
        }
        if (Neko.target_y < 0) {
            DebugLog("ClampTarget: Neko.target_y %d -> 0 (root)\n", Neko.target_y);
            Neko.target_y = 0;
        }
        int max_y = WindowHeight > BITMAP_HEIGHT ? WindowHeight - BITMAP_HEIGHT : 0;
        if (Neko.target_y > max_y) {
            DebugLog("ClampTarget: Neko.target_y %d -> %d (root)\n", Neko.target_y, max_y);
            Neko.target_y = max_y;
        }
        return;
    }

    int monitor = -1;
    if (Config.restrict_monitor >= 0 && Config.restrict_monitor < MonitorCount) {
        monitor = Config.restrict_monitor;
    } else {
        monitor = FindMonitorFor(Neko.target_x, Neko.target_y);
        if (monitor < 0) {
            monitor = FindMonitorFor(Neko.x + BITMAP_WIDTH / 2, Neko.y + BITMAP_HEIGHT);
        }
        if (monitor < 0) {
            monitor = 0;
        }
    }

    MonitorBounds bounds = GetMonitorBounds(monitor);
    
    if (Neko.target_x < bounds.min_x) {
        DebugLog("ClampTarget: Neko.target_x %d -> %d (monitor %d)\n",
                Neko.target_x, bounds.min_x, monitor);
        Neko.target_x = bounds.min_x;
    } else if (Neko.target_x > bounds.max_x) {
        DebugLog("ClampTarget: Neko.target_x %d -> %d (monitor %d)\n",
                Neko.target_x, bounds.max_x, monitor);
        Neko.target_x = bounds.max_x;
    }

    if (Neko.target_y < bounds.min_y) {
        DebugLog("ClampTarget: Neko.target_y %d -> %d (monitor %d)\n",
                Neko.target_y, bounds.min_y, monitor);
        Neko.target_y = bounds.min_y;
    } else if (Neko.target_y > bounds.max_y) {
        DebugLog("ClampTarget: Neko.target_y %d -> %d (monitor %d)\n",
                Neko.target_y, bounds.max_y, monitor);
        Neko.target_y = bounds.max_y;
    }

    if (!RectOnMonitor(Neko.target_x, Neko.target_y, BITMAP_WIDTH, BITMAP_HEIGHT)) {
        DebugLog("ClampTarget: Target (%d,%d) still invalid for monitor %d, resetting to (%d,%d)\n",
                Neko.target_x, Neko.target_y, monitor,
                bounds.min_x, bounds.min_y);
        Neko.target_x = bounds.min_x;
        Neko.target_y = bounds.min_y;
    }
}

void
Interval(void)
{
    pause();
    if (Neko.raise_window_delay > 0) {
        Neko.raise_window_delay--;
    }
}

void
TickCount(void)
{
    if (++Neko.tick_count >= MAX_TICK) {
        Neko.tick_count = 0;
    }

    if (Neko.tick_count % 2 == 0) {
        if (Neko.state_count < MAX_TICK) {
            Neko.state_count++;
        }
    }
}

void
SetNekoState(int SetValue)
{
    Neko.tick_count = 0;
    Neko.state_count = 0;
    Neko.state = SetValue;
}

void
NekoDirection(void)
{
    int NewState;
    double LargeX, LargeY;
    double Length;
    double SinTheta;

    if (Neko.move_dx == 0 && Neko.move_dy == 0) {
        NewState = NEKO_STOP;
    } else {
        LargeX = (double)Neko.move_dx;
        LargeY = (double)(-Neko.move_dy);
        Length = sqrt(LargeX * LargeX + LargeY * LargeY);
        SinTheta = LargeY / Length;

        if (Neko.move_dx > 0) {
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

    if (Neko.state != NewState) {
        SetNekoState(NewState);
    }
}

Bool
IsWindowOver(void)
{
    Bool ReturnValue = False;

    if (Neko.y <= 0) {
        Neko.y = 0;
        ReturnValue = True;
    } else if (Neko.y >= WindowHeight - BITMAP_HEIGHT) {
        Neko.y = WindowHeight - BITMAP_HEIGHT;
        ReturnValue = True;
    }
    if (Neko.x <= 0) {
        Neko.x = 0;
        ReturnValue = True;
    } else if (Neko.x >= WindowWidth - BITMAP_WIDTH) {
        Neko.x = WindowWidth - BITMAP_WIDTH;
        ReturnValue = True;
    }

    return ReturnValue;
}

Bool
IsNekoDontMove(void)
{
    return Neko.x == Neko.last_x && Neko.y == Neko.last_y;
}

Bool
IsNekoMoveStart(void)
{
    return Neko.move_dx != 0 || Neko.move_dy != 0;
}

void
PickRandomTarget(void)
{
    int m = -1;

    if (MonitorCount <= 0) {
        int width = WindowWidth > BITMAP_WIDTH ? WindowWidth - BITMAP_WIDTH : 0;
        int height = WindowHeight > BITMAP_HEIGHT ? WindowHeight - BITMAP_HEIGHT : 0;

        Neko.target_x = rand() % (width + 1);
        Neko.target_y = rand() % (height + 1);
        DebugLog("PickRandomTarget: root target=(%d,%d) width=%d height=%d\n",
                 Neko.target_x, Neko.target_y, width, height);
        return;
    }

    if (Config.restrict_monitor >= 0 && Config.restrict_monitor < MonitorCount) {
        m = Config.restrict_monitor;
    } else {
        m = rand() % MonitorCount;
    }

    MonitorBounds bounds = GetMonitorBounds(m);

    int width = bounds.max_x - bounds.min_x;
    int height = bounds.max_y - bounds.min_y;

    Neko.target_x = bounds.min_x + rand() % (width + 1);
    Neko.target_y = bounds.min_y + rand() % (height + 1);

    DebugLog("PickRandomTarget: monitor=%d target=(%d,%d) width=%d height=%d\n",
            m, Neko.target_x, Neko.target_y, width, height);
}

void
PickNearbyTarget(int radius)
{
    int x, y;

    for (int tries = 0; tries < 100; tries++) {
        x = Neko.x + (rand() % (radius * 2 + 1) - radius);
        y = Neko.y + (rand() % (radius * 2 + 1) - radius);

        if (RectOnMonitor(x, y, BITMAP_WIDTH, BITMAP_HEIGHT)) {
            Neko.target_x = x;
            Neko.target_y = y;
            DebugLog("PickNearbyTarget: target=(%d,%d) tries=%d\n", Neko.target_x, Neko.target_y, tries + 1);
            return;
        }
    }

    PickRandomTarget();
    DebugLog("PickNearbyTarget: fallback target=(%d,%d)\n", Neko.target_x, Neko.target_y);
}

void
CalcDxDy(void)
{
    double LargeX, LargeY;
    double DoubleLength, Length;

    if (DebugMode) {
        DebugLog("CalcDxDy: start Target=(%d,%d) Neko=(%d,%d) Neko.waiting=%d Neko.zoomies=%d\n",
                 Neko.target_x, Neko.target_y, Neko.x, Neko.y, Neko.waiting, Neko.zoomies);
    }

    if (Neko.zoomies && time(NULL) > Neko.zoomies_end_time) {
        Neko.zoomies = 0;
    }

    if (Neko.waiting) {
            if (time(NULL) < Neko.next_move_time) {
                Neko.move_dx = 0;
                Neko.move_dy = 0;
                return;
            }

            Neko.waiting = 0;
            Bed.going_to_bed = 0;

            if (Neko.zoomies) {
                PickRandomTarget();
            } else {
                int r = rand() % 100;

            if (Bed.enabled && r < 15) {
                /* Bed.x/Bed.y are stored as top-left; target should be top-left too. */
                Neko.target_x = Bed.x;
                Neko.target_y = Bed.y;
                Bed.going_to_bed = 1;
            } else if (r < (Bed.enabled ? 55 : 40)) {
                PickNearbyTarget(300);
            } else if (r < (Bed.enabled ? 85 : 70)) {
                PickRandomTarget();
            } else {
                Neko.target_x = Neko.x;
                Neko.target_y = Neko.y;
            }
        }

            if (!Neko.zoomies && rand() % 100 < 5) {
            Neko.zoomies = 1;
            Neko.zoomies_end_time = time(NULL) + 50;
        }

            DebugLog("CalcDxDy: selected target=(%d,%d) Bed.going_to_bed=%d Neko.zoomies=%d Neko=(%d,%d)\n",
                     Neko.target_x, Neko.target_y, Bed.going_to_bed, Neko.zoomies, Neko.x, Neko.y);
        ClampTarget();
    }

    int ReachDistance = 2;

    /* Consider Neko reached the target when its top-left aligns (within tolerance).
       This handles edge cases where IsWindowOver clamps Neko positions at screen edges. */
    if (!Neko.waiting &&
        abs(Neko.target_x - Neko.x) <= ReachDistance &&
        abs(Neko.target_y - Neko.y) <= ReachDistance) {
        int delay;

        Neko.waiting = 1;
        delay = (int)(Config.min_wait + rand() % (Config.max_wait - Config.min_wait + 1));

        if (Bed.going_to_bed) {
            delay *= 2;
        }
        if (Neko.zoomies) {
            delay = 1 + rand() % 3;
        }

        Neko.next_move_time = time(NULL) + delay;
        Neko.move_dx = 0;
        Neko.move_dy = 0;
        if (Neko.state != NEKO_STOP) {
            SetNekoState(NEKO_STOP);
        }
        return;
    }

    /* Use top-left coordinates for both Target and Neko so edges align correctly. */
    LargeX = (double)(Neko.target_x - Neko.x);
    LargeY = (double)(Neko.target_y - Neko.y);

    DoubleLength = LargeX * LargeX + LargeY * LargeY;
    if (DoubleLength != 0.0) {
        Length = sqrt(DoubleLength);
        if (Length <= Config.speed) {
            Neko.move_dx = (int)LargeX;
            Neko.move_dy = (int)LargeY;
        } else {
            Neko.move_dx = (int)((Config.speed * LargeX) / Length);
            Neko.move_dy = (int)((Config.speed * LargeY) / Length);
        }

        if (Neko.move_dx == 0 && Neko.move_dy == 0) {
            Neko.move_dx = LargeX > 0 ? 1 : (LargeX < 0 ? -1 : 0);
            Neko.move_dy = LargeY > 0 ? 1 : (LargeY < 0 ? -1 : 0);
            if (Neko.move_dx == 0 && Neko.move_dy == 0) {
                Neko.move_dx = 1;
            }
            DebugLog("CalcDxDy: forced minimal move=(%d,%d) for vector=(%.2f,%.2f)\n",
                     Neko.move_dx, Neko.move_dy, LargeX, LargeY);
        }
    } else {
        Neko.move_dx = 0;
        Neko.move_dy = 0;
    }

    /* If movement vector is exactly zero (e.g., due to centering offsets),
       treat as having reached the target so Neko doesn't get stuck. */
    if (!Neko.waiting && Neko.move_dx == 0 && Neko.move_dy == 0) {
        int delay;

        Neko.waiting = 1;
        delay = (int)(Config.min_wait + rand() % (Config.max_wait - Config.min_wait + 1));

        if (Bed.going_to_bed) {
            delay *= 2;
        }
        if (Neko.zoomies) {
            delay = 1 + rand() % 3;
        }

        Neko.next_move_time = time(NULL) + delay;
        if (Neko.state != NEKO_STOP) {
            SetNekoState(NEKO_STOP);
        }
        return;
    }

    if (DebugMode) {
        DebugLog("CalcDxDy: vector=(%.2f,%.2f) speed=%.2f move=(%d,%d)\n",
                 LargeX, LargeY, Config.speed, Neko.move_dx, Neko.move_dy);
    }
}

void
NekoThinkDraw(void)
{
    CalcDxDy();

    if (Neko.state != NEKO_SLEEP) {
        DrawNeko(Neko.x, Neko.y, Neko.tick_count & 0x1);
    } else {
        DrawNeko(Neko.x, Neko.y, (Neko.tick_count >> 2) & 0x1);
    }

    TickCount();

    switch (Neko.state) {
    case NEKO_STOP:
        if (IsNekoMoveStart()) {
            SetNekoState(NEKO_AWAKE);
            break;
        }
        if (Neko.state_count < NEKO_STOP_TIME) {
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
        if (Neko.move_dx < 0 && Neko.x <= 0) {
            SetNekoState(NEKO_L_TOGI);
        } else if (Neko.move_dx > 0 && Neko.x >= WindowWidth - BITMAP_WIDTH) {
            SetNekoState(NEKO_R_TOGI);
        } else if (Neko.move_dy < 0 && Neko.y <= 0) {
            SetNekoState(NEKO_U_TOGI);
        } else if (Neko.move_dy > 0 && Neko.y >= WindowHeight - BITMAP_HEIGHT) {
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
        if (Neko.state_count < NEKO_JARE_TIME) {
            break;
        }
        SetNekoState(NEKO_KAKI);
        break;
    case NEKO_KAKI:
        if (IsNekoMoveStart()) {
            SetNekoState(NEKO_AWAKE);
            break;
        }
        if (Neko.state_count < NEKO_KAKI_TIME) {
            break;
        }
        SetNekoState(NEKO_AKUBI);
        break;
    case NEKO_AKUBI:
        if (IsNekoMoveStart()) {
            SetNekoState(NEKO_AWAKE);
            break;
        }
        if (Neko.state_count < NEKO_AKUBI_TIME) {
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
        if (Neko.state_count < NEKO_AWAKE_TIME) {
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
        Neko.x += Neko.move_dx;
        Neko.y += Neko.move_dy;
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
        if (Neko.state_count < NEKO_TOGI_TIME) {
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
        Neko.x = (int)WindowWidth / 2;
        Neko.y = (int)WindowHeight / 2;
        Neko.target_x = ForceTargetX;
        Neko.target_y = ForceTargetY;
        /* ensure forced target is clamped to a valid location */
        ClampTarget();
        Neko.last_x = Neko.x;
        Neko.last_y = Neko.y;
        Neko.waiting = 0; /* start moving immediately toward forced target */
        DebugLog("ProcessNeko: Forced target=(%d,%d) start=(%d,%d)\n", Neko.target_x, Neko.target_y, Neko.x, Neko.y);
    } else {
        Neko.x = Neko.target_x;
        Neko.y = Neko.target_y;
        Neko.last_x = Neko.x;
        Neko.last_y = Neko.y;
        Neko.waiting = 1;
        Neko.next_move_time = time(NULL);
        SetNekoState(NEKO_STOP);
    }

    timerclear(&Value.it_interval);
    timerclear(&Value.it_value);

    Value.it_interval.tv_sec = Config.interval_time / 1000000L;
    Value.it_interval.tv_usec = Config.interval_time % 1000000L;
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
