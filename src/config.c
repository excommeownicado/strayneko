#include "oneko.h"

int
ParseLongOption(const char *option, const char *value, long minimum,
                long *result)
{
    char *end;
    long parsed;

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

    if (Foreground == NULL) {
        if ((resource = NekoGetDefault("foreground")) != NULL) {
            Foreground = resource;
        }
    }

    if (Background == NULL) {
        if ((resource = NekoGetDefault("background")) != NULL) {
            Background = resource;
        }
    }

    if (IntervalTime == 0) {
        if ((resource = NekoGetDefault("time")) != NULL) {
            ParseLongOption("time resource", resource, 1, &IntervalTime);
        }
    }

    if (NekoSpeed == 0.0) {
        if ((resource = NekoGetDefault("speed")) != NULL) {
            ParseDoubleOption("speed resource", resource, 0.0, &NekoSpeed);
        }
    }

    if (NoShape == NOTDEFINED) {
        if ((resource = NekoGetDefault("noshape")) != NULL) {
            NoShape = IsTrue(resource);
        }
    }

    if (ReverseVideo == NOTDEFINED) {
        if ((resource = NekoGetDefault("reverse")) != NULL) {
            ReverseVideo = IsTrue(resource);
        }
    }

    if (Foreground == NULL) {
        Foreground = DEFAULT_FOREGROUND;
    }
    if (Background == NULL) {
        Background = DEFAULT_BACKGROUND;
    }
    if (IntervalTime == 0) {
        IntervalTime = 125000L;
    }
    if (NekoSpeed == 0.0) {
        NekoSpeed = 13.0;
    }
    if (NoShape == NOTDEFINED) {
        NoShape = False;
    }
    if (ReverseVideo == NOTDEFINED) {
        ReverseVideo = False;
    }
}

void
SetupColors(void)
{
    XColor theExactColor;
    Colormap theColormap;

    theColormap = DefaultColormap(theDisplay, theScreen);

    if (theDepth == 1) {
        Foreground = "black";
        Background = "white";
    }

    if (ReverseVideo == True) {
        char *tmp;
        tmp = Foreground;
        Foreground = Background;
        Background = tmp;
    }

    if (!XAllocNamedColor(theDisplay, theColormap,
                          Foreground, &theForegroundColor, &theExactColor)) {
        fprintf(stderr, "%s: Can't XAllocNamedColor(\"%s\").\n",
                ProgramName, Foreground);
        exit(1);
    }

    if (!XAllocNamedColor(theDisplay, theColormap,
                          Background, &theBackgroundColor, &theExactColor)) {
        fprintf(stderr, "%s: Can't XAllocNamedColor(\"%s\").\n",
                ProgramName, Background);
        exit(1);
    }
}

void
InitMonitors(void)
{
    Window root;

    root = RootWindow(theDisplay, DefaultScreen(theDisplay));
    Monitors = XRRGetMonitors(theDisplay, root, True, &MonitorCount);
}
