#include "colors.h"
#include "strayneko.h"

void
SetupColors(void)
{
    XColor exact_color;
    Colormap colormap;

    colormap = DefaultColormap(theDisplay, theScreen);

    if (theDepth == 1) {
        Config.foreground = "black";
        Config.background = "white";
    }

    if (!XAllocNamedColor(
            theDisplay,
            colormap,
            Config.foreground,
            &theForegroundColor,
            &exact_color)) {
        fprintf(
            stderr,
            "%s: Can't XAllocNamedColor(\"%s\").\n",
            ProgramName,
            Config.foreground
        );
        exit(1);
    }

    if (!XAllocNamedColor(
            theDisplay,
            colormap,
            Config.background,
            &theBackgroundColor,
            &exact_color)) {
        fprintf(
            stderr,
            "%s: Can't XAllocNamedColor(\"%s\").\n",
            ProgramName,
            Config.background
        );
        exit(1);
    }
}
