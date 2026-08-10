#ifndef STRAYNEKO_PLATFORM_EVENT_H
#define STRAYNEKO_PLATFORM_EVENT_H

#include <stdbool.h>

/* Platform-neutral events consumed by the application core. */
typedef enum {
    PLATFORM_EVENT_NONE = 0,
    PLATFORM_EVENT_QUIT,
    PLATFORM_EVENT_REDRAW,
    PLATFORM_EVENT_BED_REDRAW,
    PLATFORM_EVENT_WINDOW_RAISE,
    PLATFORM_EVENT_BED_DRAG_START,
    PLATFORM_EVENT_BED_DRAG_MOVE,
    PLATFORM_EVENT_BED_DRAG_END
} PlatformEventType;

typedef struct {
    PlatformEventType type;
    int x;
    int y;
    int button;
} PlatformEvent;

#endif
