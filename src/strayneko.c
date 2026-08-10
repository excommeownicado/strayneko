#include "strayneko.h"
#include "platform_time.h"

#include <stdarg.h>

const char *ClassName = "Strayneko";
char *ProgramName;

NekoData Neko = {
    .x = 0,
    .y = 0,
    .target_x = 0,
    .target_y = 0,
    .last_sprite = SPRITE_COUNT
};

BedData Bed = {
    .x = 500,
    .y = 500,
    .enabled = False
};

Display *theDisplay = NULL;
Window theWindow = None;
Window BedWindow = None;
GC BedGC = NULL;
unsigned int WindowWidth = BITMAP_WIDTH;
unsigned int WindowHeight = BITMAP_HEIGHT;
int TerminationRequested = 0;
int MonitorCount = 0;
MonitorRect *Monitors = NULL;
ConfigData Config = {
    .restrict_monitor = -1
};
int ForceTargetFlag = 0;
int ForceTargetX = 0;
int ForceTargetY = 0;

#ifdef ENABLE_DEBUG
int DebugMode = 0;

static void
DebugLog(const char *format, ...)
    __attribute__((format(printf, 1, 2)));

static void
DebugLog(const char *format, ...)
{
    va_list args;

    if (!DebugMode) {
        return;
    }

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}
#else
#define DebugLog(...) ((void)0)
#endif

double SinPiPer8Times3;
double SinPiPer8;
