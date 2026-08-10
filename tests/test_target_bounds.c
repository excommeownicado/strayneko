#include <stdio.h>
#include <stdlib.h>

#include "strayneko.h"
#include "target_bounds.h"

#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "target bounds test failed: %s\n", #condition); \
            return 1; \
        } \
    } while (0)

NekoData Neko;
ConfigData Config;
int MonitorCount;
unsigned int WindowWidth;
unsigned int WindowHeight;

static MonitorBounds TestMonitors[] = {
    { .min_x = 0, .min_y = 0, .max_x = 1599, .max_y = 899 },
    { .min_x = 1600, .min_y = 0, .max_x = 3519, .max_y = 1079 }
};

int
FindMonitorFor(int x, int y)
{
    for (int i = 0; i < MonitorCount; i++) {
        if (x >= TestMonitors[i].min_x &&
            x <= TestMonitors[i].max_x &&
            y >= TestMonitors[i].min_y &&
            y <= TestMonitors[i].max_y) {
            return i;
        }
    }

    return -1;
}

MonitorBounds
GetMonitorBounds(int monitor)
{
    return TestMonitors[monitor];
}

static int
TestRootBounds(void)
{
    MonitorCount = 0;
    WindowWidth = 1920;
    WindowHeight = 1080;
    Config.restrict_monitor = -1;

    Neko.target_x = -100;
    Neko.target_y = -100;
    ClampTargetToMonitor();
    CHECK(Neko.target_x == 0);
    CHECK(Neko.target_y == 0);

    Neko.target_x = 5000;
    Neko.target_y = 5000;
    ClampTargetToMonitor();
    CHECK(Neko.target_x == 1888);
    CHECK(Neko.target_y == 1048);
    return 0;
}

static int
TestRestrictedMonitor(void)
{
    MonitorCount = 2;
    Config.restrict_monitor = 1;

    Neko.target_x = 100;
    Neko.target_y = 100;
    ClampTargetToMonitor();
    CHECK(Neko.target_x == 1600);
    CHECK(Neko.target_y == 100);

    Neko.target_x = 3400;
    Neko.target_y = 1050;
    ClampTargetToMonitor();
    CHECK(Neko.target_x == 3519);
    CHECK(Neko.target_y == 1079);
    return 0;
}

static int
TestAutomaticMonitorSelection(void)
{
    Config.restrict_monitor = -1;

    Neko.target_x = 2000;
    Neko.target_y = 500;
    Neko.x = 2000;
    Neko.y = 500;
    ClampTargetToMonitor();
    CHECK(Neko.target_x == 2000);
    CHECK(Neko.target_y == 500);

    Neko.target_x = -500;
    Neko.target_y = -500;
    Neko.x = 1700;
    Neko.y = 500;
    ClampTargetToMonitor();
    CHECK(Neko.target_x == 1600);
    CHECK(Neko.target_y == 0);
    return 0;
}

int
main(void)
{
    if (TestRootBounds() != 0 ||
        TestRestrictedMonitor() != 0 ||
        TestAutomaticMonitorSelection() != 0) {
        return EXIT_FAILURE;
    }

    puts("target bounds tests passed");
    return EXIT_SUCCESS;
}
