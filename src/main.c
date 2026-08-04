#include "strayneko.h"
#include <signal.h>
#include <time.h>

static void
NullFunction(int signum)
{
    (void)signum;
    /* No operation. */
#if defined(SYSV) || defined(SVR4)
    signal(SIGALRM, NullFunction);
#endif
}

static int
NekoErrorHandler(Display *dpy, XErrorEvent *err)
{
    char msg[80];
    XGetErrorText(dpy, err->error_code, msg, sizeof(msg));
    fprintf(stderr, "%s: Error and exit.\n%s\n", ProgramName, msg);
    exit(1);
}

static char *message[] = {
    "",
    "Options are:",
    "--monitor <number>\t: neko appears on specified monitor",
    "--fg <color>\t\t: foreground color",
    "--bg <color>\t\t: background color",
    "--speed <dots>",
    "--time <microseconds>",
    "--min-wait <seconds>\t: set minimum idle time between walks (default: 20)",
    "--max-wait <seconds>\t: set maximum idle time between walks (default: 150)",
    "--bed\t\t\t: show a draggable bed",
#ifdef ENABLE_DEBUG
    "--debug\t\t: enable debug log output to stderr",
#endif
    NULL
};

void
Usage(void)
{
    char **mptr;

    mptr = message;
    fprintf(stderr, "Usage: %s [<options>]\n", ProgramName);
    while (*mptr) {
        fprintf(stderr, "%s\n", *mptr);
        mptr++;
    }
}

void
GetArguments(int argc, char *argv[], char *theDisplayName)
{
    int ArgCounter;
    long value;

    theDisplayName[0] = '\0';

    for (ArgCounter = 0; ArgCounter < argc; ArgCounter++) {
        if (strcmp(argv[ArgCounter], "--help") == 0 ||
            strcmp(argv[ArgCounter], "-h") == 0) {
            Usage();
            exit(0);
        } else if (strcmp(argv[ArgCounter], "--monitor") == 0) {
            ArgCounter++;
            if (ArgCounter >= argc) {
                fprintf(stderr, "%s: --monitor option error.\n", ProgramName);
                exit(1);
            }
            if (!ParseLongOption("--monitor", argv[ArgCounter], 0, &value)) {
                exit(1);
            }
            if (value > INT_MAX) {
                fprintf(stderr, "%s: --monitor value is too large.\n", ProgramName);
                exit(1);
            }
            RestrictMonitor = (int)value;
        } else if (strcmp(argv[ArgCounter], "--speed") == 0) {
            ArgCounter++;
            if (ArgCounter < argc) {
                if (!ParseDoubleOption("--speed", argv[ArgCounter], 0.0, &NekoSpeed)) {
                    exit(1);
                }
            } else {
                fprintf(stderr, "%s: --speed option error.\n", ProgramName);
                exit(1);
            }
        } else if (strcmp(argv[ArgCounter], "--time") == 0) {
            ArgCounter++;
            if (ArgCounter < argc) {
                if (!ParseLongOption("--time", argv[ArgCounter], 1, &IntervalTime)) {
                    exit(1);
                }
            } else {
                fprintf(stderr, "%s: --time option error.\n", ProgramName);
                exit(1);
            }
        } else if (strcmp(argv[ArgCounter], "--fg") == 0 ||
                   strcmp(argv[ArgCounter], "--foreground") == 0) {
            ArgCounter++;
            if (ArgCounter >= argc) {
                fprintf(stderr, "%s: --fg option error.\n", ProgramName);
                exit(1);
            }
            Foreground = argv[ArgCounter];
        } else if (strcmp(argv[ArgCounter], "--bg") == 0 ||
                   strcmp(argv[ArgCounter], "--background") == 0) {
            ArgCounter++;
            if (ArgCounter >= argc) {
                fprintf(stderr, "%s: --bg option error.\n", ProgramName);
                exit(1);
            }
            Background = argv[ArgCounter];
        } else if (strcmp(argv[ArgCounter], "--noshape") == 0) {
            NoShape = True;
        } else if (strcmp(argv[ArgCounter], "--bed") == 0) {
            UseBed = True;
#ifdef ENABLE_DEBUG
        } else if (strcmp(argv[ArgCounter], "--debug") == 0) {
            DebugMode = 1;
#endif
        } else if (strcmp(argv[ArgCounter], "--force-target") == 0) {
                ArgCounter++;
                if (ArgCounter >= argc) {
                    fprintf(stderr, "%s: --force-target option error. Expected format x,y\n", ProgramName);
                    exit(1);
                }
                {
                    int fx = 0, fy = 0;
                    if (sscanf(argv[ArgCounter], "%d,%d", &fx, &fy) != 2) {
                        fprintf(stderr, "%s: --force-target value is invalid. Expected x,y\n", ProgramName);
                        exit(1);
                    }
                    ForceTargetFlag = 1;
                    ForceTargetX = fx;
                    ForceTargetY = fy;
                }
        } else if (strcmp(argv[ArgCounter], "--min-wait") == 0) {
            ArgCounter++;
            if (ArgCounter < argc) {
                if (!ParseLongOption("--min-wait", argv[ArgCounter], 0, &value) || value > INT_MAX) {
                    fprintf(stderr, "%s: --min-wait value is invalid.\n", ProgramName);
                    exit(1);
                }
                MinWait = (int)value;
            } else {
                fprintf(stderr, "%s: --min-wait option error.\n", ProgramName);
                exit(1);
            }
        } else if (strcmp(argv[ArgCounter], "--max-wait") == 0) {
            ArgCounter++;
            if (ArgCounter < argc) {
                if (!ParseLongOption("--max-wait", argv[ArgCounter], 0, &value) || value > INT_MAX) {
                    fprintf(stderr, "%s: --max-wait value is invalid.\n", ProgramName);
                    exit(1);
                }
                MaxWait = (int)value;
            } else {
                fprintf(stderr, "%s: --max-wait option error.\n", ProgramName);
                exit(1);
            }
        } else {
            fprintf(stderr, "%s: Unknown option \"%s\".\n", ProgramName, argv[ArgCounter]);
            Usage();
            exit(1);
        }
    }

    if (MaxWait < MinWait) {
        fprintf(stderr, "%s: max-wait must be >= min-wait.\n", ProgramName);
        exit(1);
    }

    if (strlen(theDisplayName) < 1) {
        theDisplayName[0] = '\0';
    }
}

int
main(int argc, char *argv[])
{
    char theDisplayName[MAXDISPLAYNAME];

    srand(time(NULL));
    ProgramName = argv[0];

    argc--;
    argv++;

    GetArguments(argc, argv, theDisplayName);

    XSetErrorHandler(NekoErrorHandler);

    InitScreen(theDisplayName);

    signal(SIGALRM, NullFunction);
    signal(SIGINT, RestoreCursorHandler);
    signal(SIGTERM, RestoreCursorHandler);
    signal(SIGQUIT, RestoreCursorHandler);

    SinPiPer8Times3 = sin(PI_PER8 * 3.0);
    SinPiPer8 = sin(PI_PER8);

    ProcessNeko();
    RestoreCursor();

    return 0;
}
