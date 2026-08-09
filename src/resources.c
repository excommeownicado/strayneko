#include "resources.h"
#include "strayneko.h"
#include "parser.h"

static char *
NekoGetDefault(const char *resource)
{
    char *value;

    value = XGetDefault(theDisplay, ProgramName, resource);
    if (value != NULL) {
        return value;
    }

    return XGetDefault(theDisplay, ClassName, resource);
}

void
GetResources(void)
{
    char *resource;

    if (Config.foreground == NULL) {
        resource = NekoGetDefault("foreground");
        if (resource != NULL) {
            Config.foreground = resource;
        }
    }

    if (Config.background == NULL) {
        resource = NekoGetDefault("background");
        if (resource != NULL) {
            Config.background = resource;
        }
    }

    if (!Config.interval_time_set) {
        resource = NekoGetDefault("time");
        if (resource != NULL) {
            if (!ParseLongOption(resource, 1, &Config.interval_time)) {
                exit(1);
            }

            Config.interval_time_set = true;
        }
    }

    if (!Config.speed_set) {
        resource = NekoGetDefault("speed");
        if (resource != NULL) {
            if (!ParseDoubleOption(resource, 0.0, &Config.speed)) {
                exit(1);
            }

            Config.speed_set = true;
        }
    }

    if (Config.no_shape == NOTDEFINED) {
        resource = NekoGetDefault("noshape");
        if (resource != NULL) {
            Config.no_shape = IsTrue(resource);
        }
    }

    if (Config.foreground == NULL) {
        Config.foreground = DEFAULT_FOREGROUND;
    }

    if (Config.background == NULL) {
        Config.background = DEFAULT_BACKGROUND;
    }

    if (!Config.interval_time_set) {
        Config.interval_time = 125000L;
        Config.interval_time_set = true;
    }

    if (!Config.speed_set) {
        Config.speed = 13.0;
        Config.speed_set = true;
    }

    if (Config.no_shape == NOTDEFINED) {
        Config.no_shape = False;
    }
}
