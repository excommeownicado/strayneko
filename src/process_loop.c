#include "strayneko.h"
#include "platform_input.h"
#include "platform_time.h"
#include "target_bounds.h"
#include "x11_platform_events.h"

static void
ProcessPendingPlatformEvents(void)
{
    PlatformEvent event;

    while (!TerminationRequested &&
           X11PollPlatformEvent(theDisplay, theWindow, BedWindow, &event)) {
        ProcessPlatformEvent(&event);
    }
}

void
ProcessNeko(void)
{
    PickRandomTarget();

    if (ForceTargetFlag) {
        Neko.x = (int)WindowWidth / 2;
        Neko.y = (int)WindowHeight / 2;
        Neko.target_x = ForceTargetX;
        Neko.target_y = ForceTargetY;
        ClampTargetToMonitor();
        Neko.last_x = Neko.x;
        Neko.last_y = Neko.y;
        Neko.waiting = 0;
    } else {
        Neko.x = Neko.target_x;
        Neko.y = Neko.target_y;
        Neko.last_x = Neko.x;
        Neko.last_y = Neko.y;
        Neko.waiting = 1;
        Neko.next_move_time = (time_t)(PlatformGetTimeMs() / 1000ULL);
        SetNekoState(NEKO_STOP);
    }

    while (!TerminationRequested) {
        ProcessPendingPlatformEvents();

        if (TerminationRequested) {
            break;
        }

        NekoThinkDraw();
    }
}
