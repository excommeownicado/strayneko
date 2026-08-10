#include "strayneko.h"
#include "platform_input.h"
#include "x11_platform_events.h"

#include <sys/time.h>

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
    struct itimerval timer;

    PickRandomTarget();

    if (ForceTargetFlag) {
        Neko.x = (int)WindowWidth / 2;
        Neko.y = (int)WindowHeight / 2;
        Neko.target_x = ForceTargetX;
        Neko.target_y = ForceTargetY;
        ClampTarget();
        Neko.last_x = Neko.x;
        Neko.last_y = Neko.y;
        Neko.waiting = 0;
        DebugLog("ProcessNeko: Forced target=(%d,%d) start=(%d,%d)\n",
                 Neko.target_x, Neko.target_y, Neko.x, Neko.y);
    } else {
        Neko.x = Neko.target_x;
        Neko.y = Neko.target_y;
        Neko.last_x = Neko.x;
        Neko.last_y = Neko.y;
        Neko.waiting = 1;
        Neko.next_move_time = time(NULL);
        SetNekoState(NEKO_STOP);
    }

    timerclear(&timer.it_interval);
    timerclear(&timer.it_value);

    timer.it_interval.tv_sec = Config.interval_time / 1000000L;
    timer.it_interval.tv_usec = Config.interval_time % 1000000L;
    timer.it_value = timer.it_interval;

    if (setitimer(ITIMER_REAL, &timer, 0) != 0) {
        perror("setitimer");
        return;
    }

    while (!TerminationRequested) {
        ProcessPendingPlatformEvents();

        if (TerminationRequested) {
            break;
        }

        NekoThinkDraw();
    }
}
