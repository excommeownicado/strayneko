#include "strayneko.h"
#include <stdlib.h>

void
PlaceBedOnMonitor(void)
{
    int monitor;
    int max_x;
    int max_y;

    if (MonitorCount <= 0) {
        Bed.x = ((int)WindowWidth - bed_width) / 2;
        Bed.y = ((int)WindowHeight - bed_height) / 2;
        return;
    }

    monitor = Config.restrict_monitor >= 0 ? Config.restrict_monitor : rand() % MonitorCount;
    max_x = Monitors[monitor].width - bed_width;
    max_y = Monitors[monitor].height - bed_height;

    Bed.x = Monitors[monitor].x + (max_x > 0 ? rand() % (max_x + 1) : 0);
    Bed.y = Monitors[monitor].y + (max_y > 0 ? rand() % (max_y + 1) : 0);
}
