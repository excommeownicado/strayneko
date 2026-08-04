#include "strayneko.h"
#include <stdlib.h>

void
PlaceBedOnMonitor(void)
{
    int monitor;
    int max_x;
    int max_y;

    if (MonitorCount <= 0) {
        BedX = ((int)WindowWidth - bed_width) / 2;
        BedY = ((int)WindowHeight - bed_height) / 2;
        return;
    }

    monitor = RestrictMonitor >= 0 ? RestrictMonitor : rand() % MonitorCount;
    max_x = Monitors[monitor].width - bed_width;
    max_y = Monitors[monitor].height - bed_height;

    BedX = Monitors[monitor].x + (max_x > 0 ? rand() % (max_x + 1) : 0);
    BedY = Monitors[monitor].y + (max_y > 0 ? rand() % (max_y + 1) : 0);
}
