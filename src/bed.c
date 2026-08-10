#include "strayneko.h"

#include <stdlib.h>

static void
ClampBedToBounds(MonitorBounds bounds)
{
    int max_x = bounds.max_x;
    int max_y = bounds.max_y;

    if (Bed.x < bounds.min_x) {
        Bed.x = bounds.min_x;
    } else if (Bed.x > max_x) {
        Bed.x = max_x;
    }

    if (Bed.y < bounds.min_y) {
        Bed.y = bounds.min_y;
    } else if (Bed.y > max_y) {
        Bed.y = max_y;
    }
}

void
ClampBedPosition(void)
{
    int monitor;

    if (!Monitors || MonitorCount <= 0) {
        int max_x = WindowWidth > BITMAP_WIDTH
            ? (int)WindowWidth - BITMAP_WIDTH
            : 0;
        int max_y = WindowHeight > BITMAP_HEIGHT
            ? (int)WindowHeight - BITMAP_HEIGHT
            : 0;

        if (Bed.x < 0) {
            Bed.x = 0;
        } else if (Bed.x > max_x) {
            Bed.x = max_x;
        }

        if (Bed.y < 0) {
            Bed.y = 0;
        } else if (Bed.y > max_y) {
            Bed.y = max_y;
        }

        return;
    }

    if (Config.restrict_monitor >= 0 &&
        Config.restrict_monitor < MonitorCount) {
        monitor = Config.restrict_monitor;
    } else {
        monitor = FindMonitorFor(
            Bed.x + BITMAP_WIDTH / 2,
            Bed.y + BITMAP_HEIGHT / 2
        );

        if (monitor < 0) {
            monitor = 0;
        }
    }

    ClampBedToBounds(GetMonitorBounds(monitor));
}

void
PlaceBedOnMonitor(void)
{
    int monitor;

    if (!Monitors || MonitorCount <= 0) {
        Bed.x = ((int)WindowWidth - BITMAP_WIDTH) / 2;
        Bed.y = ((int)WindowHeight - BITMAP_HEIGHT) / 2;
        ClampBedPosition();
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

    ClampBedPosition();
}
