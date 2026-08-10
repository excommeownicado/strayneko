#include "strayneko.h"
#include "platform.h"
#include "platform_input.h"
#include "target_bounds.h"

static void
ProcessPendingPlatformEvents(void)
{
    PlatformEvent event;

    while (!TerminationRequested && PlatformPollEvent(&event)) {
        ProcessPlatformEvent(&event);
    }
}

static void
InitializeForcedTarget(void)
{
    Neko.target_x = ForceTargetX;
    Neko.target_y = ForceTargetY;
    ClampTargetToMonitor();

    if (Config.restrict_monitor >= 0 &&
        Config.restrict_monitor < MonitorCount) {
        MonitorBounds bounds = GetMonitorBounds(Config.restrict_monitor);

        Neko.x = bounds.min_x +
            (bounds.max_x - bounds.min_x) / 2;
        Neko.y = bounds.min_y +
            (bounds.max_y - bounds.min_y) / 2;
    } else {
        Neko.x = (int)WindowWidth / 2;
        Neko.y = (int)WindowHeight / 2;
    }

    Neko.last_x = Neko.x;
    Neko.last_y = Neko.y;
    Neko.waiting = 0;
    Neko.move_dx = 0;
    Neko.move_dy = 0;
}

void
ProcessNeko(void)
{
    PickRandomTarget();

    if (ForceTargetFlag) {
        InitializeForcedTarget();
    } else {
        Neko.x = Neko.target_x;
        Neko.y = Neko.target_y;
        Neko.last_x = Neko.x;
        Neko.last_y = Neko.y;
        Neko.waiting = 1;
        /* CalcDxDy() uses time(NULL), so keep the initial deadline on the
         * same wall-clock scale instead of mixing it with monotonic ms. */
        Neko.next_move_time = time(NULL);
        SetNekoState(NEKO_STOP);
    }

    while (!TerminationRequested) {
        ProcessPendingPlatformEvents();

        if (TerminationRequested) {
            break;
        }

        /* NekoThinkDraw() owns the frame interval via Interval(). */
        NekoThinkDraw();
    }
}
