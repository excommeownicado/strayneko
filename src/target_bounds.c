#include "target_bounds.h"
#include "strayneko.h"

static Rect
MonitorBoundsToRect(MonitorBounds bounds)
{
    return (Rect){
        .x = bounds.min_x,
        .y = bounds.min_y,
        .width = bounds.max_x - bounds.min_x + BITMAP_WIDTH,
        .height = bounds.max_y - bounds.min_y + BITMAP_HEIGHT
    };
}

void
ClampTargetToMonitor(void)
{
    MonitorBounds bounds;
    int monitor;

    if (MonitorCount <= 0) {
        Rect root = {
            .x = 0,
            .y = 0,
            .width = (int)WindowWidth,
            .height = (int)WindowHeight
        };
        Rect target = {
            .x = Neko.target_x,
            .y = Neko.target_y,
            .width = BITMAP_WIDTH,
            .height = BITMAP_HEIGHT
        };

        target = ClampRectToRect(target, root);
        Neko.target_x = target.x;
        Neko.target_y = target.y;
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

    Rect target = {
        .x = Neko.target_x,
        .y = Neko.target_y,
        .width = BITMAP_WIDTH,
        .height = BITMAP_HEIGHT
    };

    target = ClampRectToRect(target, MonitorBoundsToRect(bounds));

    Neko.target_x = target.x;
    Neko.target_y = target.y;
}
