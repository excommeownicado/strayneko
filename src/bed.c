#include "strayneko.h"
#include <stdlib.h>

void
PlaceBedOnMonitor(void)
{
    int monitor;

    if (!Monitors || MonitorCount <= 0) {
        Bed.x = ((int)WindowWidth - bed_width) / 2;
        Bed.y = ((int)WindowHeight - bed_height) / 2;
        return;
    }

    if (Config.restrict_monitor >= 0 &&
        Config.restrict_monitor < MonitorCount) {
        monitor = Config.restrict_monitor;
    } else {
        monitor = rand() % MonitorCount;
    }

    MonitorBounds bounds = GetMonitorBounds(monitor);

    Bed.x = bounds.min_x +
        (bounds.max_x > bounds.min_x
            ? rand() % (bounds.max_x - bounds.min_x + 1)
            : 0);

    Bed.y = bounds.min_y +
        (bounds.max_y > bounds.min_y
            ? rand() % (bounds.max_y - bounds.min_y + 1)
            : 0);
}