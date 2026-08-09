#include "strayneko.h"

MonitorRect
GetMonitorRect(int monitor)
{
    MonitorRect empty = {0};

    if (!Monitors ||
        monitor < 0 ||
        monitor >= MonitorCount) {
        return empty;
    }

    return (MonitorRect) {
        .x = Monitors[monitor].x,
        .y = Monitors[monitor].y,
        .width = Monitors[monitor].width,
        .height = Monitors[monitor].height
    };
}

int
RectOnMonitor(int x, int y, int w, int h)
{
    int i;

    if (w < 0 || h < 0) {
        return 0;
    }

    int64_t right = (int64_t)x + w;
    int64_t bottom = (int64_t)y + h;

    if (MonitorCount <= 0) {
        return x >= 0 &&
               y >= 0 &&
               right <= WindowWidth &&
               bottom <= WindowHeight;
    }

    if (Config.restrict_monitor >= 0 &&
        Config.restrict_monitor < MonitorCount) {

        MonitorRect rect = GetMonitorRect(Config.restrict_monitor);

        int64_t monitor_right =
            (int64_t)rect.x + rect.width;

        int64_t monitor_bottom =
            (int64_t)rect.y + rect.height;

        return x >= rect.x &&
               y >= rect.y &&
               right <= monitor_right &&
               bottom <= monitor_bottom;
    }

    for (i = 0; i < MonitorCount; i++) {
        MonitorRect rect = GetMonitorRect(i);

        int64_t monitor_right =
            (int64_t)rect.x + rect.width;

        int64_t monitor_bottom =
            (int64_t)rect.y + rect.height;

        if (x >= rect.x &&
            y >= rect.y &&
            right <= monitor_right &&
            bottom <= monitor_bottom) {
            return 1;
        }
    }

    return 0;
}

int
FindMonitorFor(int x, int y)
{
    for (int i = 0; i < MonitorCount; i++) {
        MonitorRect rect = GetMonitorRect(i);

        int64_t right = (int64_t)rect.x + rect.width;
        int64_t bottom = (int64_t)rect.y + rect.height;

        if (x >= rect.x &&
            y >= rect.y &&
            (int64_t)x < right &&
            (int64_t)y < bottom) {
            return i;
        }
    }

    return -1;
}

MonitorBounds
GetMonitorBounds(int monitor)
{
    MonitorBounds bounds = {0};
    
    if (!Monitors ||
        monitor < 0 ||
        monitor >= MonitorCount) {
        return bounds;
    }

    MonitorRect rect = GetMonitorRect(monitor);

    bounds.min_x = rect.x;
    bounds.min_y = rect.y;
    
    int64_t max_x = (int64_t)rect.x +
                rect.width -
                BITMAP_WIDTH;

    int64_t max_y = (int64_t)rect.y +
                rect.height -
                BITMAP_HEIGHT;

    bounds.max_x = max_x > rect.x
        ? (int)max_x
        : rect.x;

    bounds.max_y = max_y > rect.y
        ? (int)max_y
        : rect.y;

    if (bounds.max_x < bounds.min_x) {
        bounds.max_x = bounds.min_x;
    }

    if (bounds.max_y < bounds.min_y) {
        bounds.max_y = bounds.min_y;
    }

    return bounds;
}