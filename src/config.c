#include "strayneko.h"
#include <sys/stat.h>

static char *
GetBedConfigPath(void)
{
    char *config_home;
    char *home;
    static char path[PATH_MAX];

    config_home = getenv("XDG_CONFIG_HOME");
    if (config_home != NULL && config_home[0] != '\0') {
        snprintf(path, sizeof(path), "%s/strayneko/bedpos", config_home);
        return path;
    }

    home = getenv("HOME");
    if (home != NULL && home[0] != '\0') {
        snprintf(path, sizeof(path), "%s/.config/strayneko/bedpos", home);
        return path;
    }

    return NULL;
}

static int
MakeDirectoryPath(const char *path)
{
    char dir[PATH_MAX];
    char *slash;
    struct stat st;

    if (path == NULL || path[0] == '\0') {
        return 0;
    }

    strncpy(dir, path, sizeof(dir));
    dir[sizeof(dir) - 1] = '\0';
    slash = strrchr(dir, '/');
    if (slash == NULL) {
        return 0;
    }
    *slash = '\0';

    for (char *p = dir + 1; *p != '\0'; p++) {
        if (*p != '/') {
            continue;
        }
        *p = '\0';
        if (stat(dir, &st) != 0) {
            if (mkdir(dir, 0700) != 0 && errno != EEXIST) {
                return 0;
            }
        } else if (!S_ISDIR(st.st_mode)) {
            return 0;
        }
        *p = '/';
    }

    if (stat(dir, &st) != 0) {
        if (mkdir(dir, 0700) != 0 && errno != EEXIST) {
            return 0;
        }
    } else if (!S_ISDIR(st.st_mode)) {
        return 0;
    }

    return 1;
}

Bool
LoadBedPosition(void)
{
    char *path;
    FILE *file;
    int x;
    int y;

    path = GetBedConfigPath();
    if (path == NULL) {
        return False;
    }

    file = fopen(path, "r");
    if (file == NULL) {
        return False;
    }

    if (fscanf(file, "%d %d", &x, &y) != 2) {
        fclose(file);
        return False;
    }

    fclose(file);

    Bed.x = x;
    Bed.y = y;
    return True;
}

void
SaveBedPosition(void)
{
    char *path;
    FILE *file;

    path = GetBedConfigPath();
    if (path == NULL) {
        return;
    }

    if (!MakeDirectoryPath(path)) {
        return;
    }

    file = fopen(path, "w");
    if (file == NULL) {
        return;
    }

    if (fprintf(file, "%d %d\n", Bed.x, Bed.y) < 0) {
        fclose(file);
        return;
    }
}

int
ParseLongOption(const char *option, const char *value, long minimum,
                long *result)
{
    char *end;
    long parsed;

    if (value == NULL) {
    fprintf(stderr, "%s: missing value for %s.\n",
            ProgramName, option);
    return False;
    }

    errno = 0;
    parsed = strtol(value, &end, 10);
    if (errno == ERANGE || *value == '\0' || *end != '\0' ||
        parsed < minimum) {
        fprintf(stderr, "%s: %s must be an integer >= %ld.\n", ProgramName,
                option, minimum);
        return False;
    }

    *result = parsed;
    return True;
}

int
ParseDoubleOption(const char *option, const char *value, double minimum,
                  double *result)
{
    char *end;
    double parsed;

    if (value == NULL) {
    fprintf(stderr, "%s: missing value for %s.\n",
            ProgramName, option);
    return False;
    }

    errno = 0;
    parsed = strtod(value, &end);
    if (errno == ERANGE || *value == '\0' || *end != '\0' ||
        parsed <= minimum) {
        fprintf(stderr, "%s: %s must be greater than %g.\n", ProgramName,
                option, minimum);
        return False;
    }

    *result = parsed;
    return True;
}

static char *
NekoGetDefault(char *resource)
{
    char *value;

    if ((value = XGetDefault(theDisplay, ProgramName, resource)) != NULL) {
        return value;
    }
    if ((value = XGetDefault(theDisplay, ClassName, resource)) != NULL) {
        return value;
    }
    return NULL;
}

void
GetResources(void)
{
    char *resource;

    if (Config.foreground == NULL) {
        if ((resource = NekoGetDefault("foreground")) != NULL) {
            Config.foreground = resource;
        }
    }

    if (Config.background == NULL) {
        if ((resource = NekoGetDefault("background")) != NULL) {
            Config.background = resource;
        }
    }

    if (Config.interval_time == 0) {
        if ((resource = NekoGetDefault("time")) != NULL) {
            ParseLongOption("time resource", resource, 1, &Config.interval_time);
        }
    }

    if (Config.speed == 0.0) {
        if ((resource = NekoGetDefault("speed")) != NULL) {
            ParseDoubleOption("speed resource", resource, 0.0, &Config.speed);
        }
    }

    if (Config.no_shape == NOTDEFINED) {
        if ((resource = NekoGetDefault("noshape")) != NULL) {
            Config.no_shape = IsTrue(resource);
        }
    }

    if (Config.reverse_video == NOTDEFINED) {
        if ((resource = NekoGetDefault("reverse")) != NULL) {
            Config.reverse_video = IsTrue(resource);
        }
    }

    if (Config.foreground == NULL) {
        Config.foreground = DEFAULT_FOREGROUND;
    }
    if (Config.background == NULL) {
        Config.background = DEFAULT_BACKGROUND;
    }
    if (Config.interval_time == 0) {
        Config.interval_time = 125000L;
    }
    if (Config.speed == 0.0) {
        Config.speed = 13.0;
    }
    if (Config.no_shape == NOTDEFINED) {
        Config.no_shape = False;
    }
    if (Config.reverse_video == NOTDEFINED) {
        Config.reverse_video = False;
    }
}

void
SetupColors(void)
{
    XColor theExactColor;
    Colormap theColormap;

    theColormap = DefaultColormap(theDisplay, theScreen);

    if (theDepth == 1) {
        Config.foreground = "black";
        Config.background = "white";
    }

    char *foreground = Config.foreground;
    char *background = Config.background;

    if (Config.reverse_video) {
        char *tmp = foreground;
        foreground = background;
        background = tmp;
    }

    if (!XAllocNamedColor(theDisplay, theColormap,
                        foreground, &theForegroundColor, &theExactColor)) {
        fprintf(stderr, "%s: Can't XAllocNamedColor(\"%s\").\n",
                ProgramName, foreground);
        exit(1);
    }

    if (!XAllocNamedColor(theDisplay, theColormap,
                        background, &theBackgroundColor, &theExactColor)) {
        fprintf(stderr, "%s: Can't XAllocNamedColor(\"%s\").\n",
                ProgramName, background);
        exit(1);
    }
}

void
InitMonitors(void)
{
    Window root;

    root = RootWindow(theDisplay, DefaultScreen(theDisplay));
    Monitors = XRRGetMonitors(theDisplay, root, True, &MonitorCount);

    if (!Monitors) {
        MonitorCount = 0;
    }
}
