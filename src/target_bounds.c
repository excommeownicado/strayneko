#include "target_bounds.h"
#include "strayneko.h"

void
ClampTargetToMonitor(void)
{
    MonitorBounds bounds;
    int monitor;

    if (MonitorCount <= 0) {
        int max_x = WindowWidth > BITMAP_WIDTH
            ? (int)WindowWidth - BITMAP_WIDTH
            : 0;
        int max_y = WindowHeight > BITMAP_HEIGHT
            ? (int)WindowHeight - BITMAP_HEIGHT
            : 0;

        if (Neko.target_x < 0) {
            Neko.target_x = 0;
        } else if (Neko.target_x > max_x) {
            Neko.target_x = max_x;
        }

        if (Neko.target_y < 0) {
            Neko.target_y = 0;
        } else if (Neko.target_y > max_y) {
            Neko.target_y = max_y;
        }

        return;
    }

    if (Config.restrict_monitor >= 0 &&
        Config.restrict_monitor < MonitorCount) {
        monitor = Config.restrict_monitor;
    } else {
        monitor = FindMonitorFor(Neko.target_x, Neko.target_y);

        if (monitor < 0) {
            monitor = FindMonitorFor(
                Neko.x + BITMAP_WIDTH / 2,
                Neko.y + BITMAP_HEIGHT
            );
        }

        if (monitor < 0) {
            monitor = 0;
        }
    }

    bounds = GetMonitorBounds(monitor);

    if (Neko.target_x < bounds.min_x) {
        Neko.target_x = bounds.min_x;
    } else if (Neko.target_x > bounds.max_x) {
        Neko.target_x = bounds.max_x;
    }

    if (Neko.target_y < bounds.min_y) {
        Neko.target_y = bounds.min_y;
    } else if (Neko.target_y > bounds.max_y) {
        Neko.target_y = bounds.max_y;
    }

    if (!RectOnMonitor(
            Neko.target_x,
            Neko.target_y,
            BITMAP_WIDTH,
            BITMAP_HEIGHT)) {
        Neko.target_x = bounds.min_x;
        Neko.target_y = bounds.min_y;
    }
}
