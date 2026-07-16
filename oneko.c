/*
 *	oneko  -  X11 $@G-(J
 */

#ifndef	lint
static char rcsid[] = "$Header: /home/sun/unix/kato/xsam/oneko/oneko.c,v 1.5 90/10/19 21:25:16 kato Exp $";
#endif

#include <unistd.h>
#include "oneko.h"
#include "patchlevel.h"
#include <time.h>
#include <X11/extensions/Xrandr.h>

/*
 *	$@%0%m!<%P%kJQ?t(J
 */

char	*ClassName = "Oneko";		/* $@%3%^%s%IL>>N(J */
char	*ProgramName;			/* $@%3%^%s%IL>>N(J */

Display	*theDisplay;			/* $@%G%#%9%W%l%$9=B$BN(J */
int	theScreen;			/* $@%9%/%j!<%sHV9f(J */
unsigned int	theDepth;		/* $@%G%W%9(J */
Window	theRoot;			/* $@%k!<%H%&%#%s%I%&$N#I#D(J */
Window	theWindow;			/* $@G-%&%#%s%I%&$N#I#D(J */

Window BedWindow;

GC BedGC;
Pixmap BedPixmap;
Pixmap BedMask;

unsigned int	WindowWidth;		/* $@%k!<%H%&%#%s%I%&$NI}(J */
unsigned int	WindowHeight;		/* $@%k!<%H%&%#%s%I%&$N9b$5(J */

XColor	theForegroundColor;		/* $@?'(J ($@%U%)%"%0%i%&%s%I(J) */
XColor	theBackgroundColor;		/* $@?'(J ($@%P%C%/%0%i%&%s%I(J) */

int Synchronous = False;
volatile sig_atomic_t TerminationRequested = 0;
/*
 *	$@$$$m$$$m$J=i4|@_Dj(J ($@%*%W%7%g%s!"%j%=!<%9$GJQ$($i$l$k$h(J)
 */

					/* Resource:	*/
char	*Foreground = NULL;		/*   foreground	*/
char	*Background = NULL;		/*   background	*/
long	IntervalTime = 0L;		/*   time	*/
double	NekoSpeed = (double)0;		/*   speed	*/
int	NoShape = NOTDEFINED;		/*   noshape	*/
int	ReverseVideo = NOTDEFINED;	/*   reverse	*/
/*
 *	$@$$$m$$$m$J>uBVJQ?t(J
 */

Bool	DontMapped = True;

int	NekoTickCount;		/* $@G-F0:n%+%&%s%?(J */
int	NekoStateCount;		/* $@G-F10l>uBV%+%&%s%?(J */
int	NekoState;		/* $@G-$N>uBV(J */

XRRMonitorInfo *Monitors = NULL;
int MonitorCount = 0;

int RestrictMonitor = -1;

int TargetX;
int TargetY;

int BedX = 500;
int BedY = 500;

int GoingToBed;
Bool UseBed = False;

int DraggingBed = 0;
int DragOffsetX = 0;
int DragOffsetY = 0;

int Waiting = 0;
time_t NextMoveTime = 0;

int Zoomies = 0;
time_t ZoomiesEndTime = 0;

int MinWait = 20;
int MaxWait = 150;

int	NekoX;			/* $@G-#X:BI8(J */
int	NekoY;			/* $@G-#Y:BI8(J */

int	NekoMoveDx;		/* $@G-0\F05wN%#X(J */
int	NekoMoveDy;		/* $@G-0\F05wN%#Y(J */

int	NekoLastX;		/* $@G-:G=*IA2h#X:BI8(J */
int	NekoLastY;		/* $@G-:G=*IA2h#Y:BI8(J */
GC	NekoLastGC;		/* $@G-:G=*IA2h(J GC */
/* Variables used to set how quickly the program will chose to raise itself. */
/* Look at Interval(), Handle Visiblility Notify Event */
#define DEFAULT_RAISE_WAIT 16  /* About 2 seconds with default interval */
int     RaiseWindowDelay=0;
/*
 *	$@$=$NB>(J
 */

double	SinPiPer8Times3;	/* sin($@#3&P!?#8(J) */
double	SinPiPer8;		/* sin($@&P!?#8(J) */

Pixmap	Mati2Xbm, Jare2Xbm, Kaki1Xbm, Kaki2Xbm, Mati3Xbm, Sleep1Xbm, Sleep2Xbm;
Pixmap	Mati2Msk, Jare2Msk, Kaki1Msk, Kaki2Msk, Mati3Msk, Sleep1Msk, Sleep2Msk;

Pixmap	AwakeXbm, AwakeMsk;

Pixmap	Up1Xbm, Up2Xbm, Down1Xbm, Down2Xbm, Left1Xbm, Left2Xbm;
Pixmap	Up1Msk, Up2Msk, Down1Msk, Down2Msk, Left1Msk, Left2Msk;
Pixmap	Right1Xbm, Right2Xbm, UpLeft1Xbm, UpLeft2Xbm, UpRight1Xbm;
Pixmap	Right1Msk, Right2Msk, UpLeft1Msk, UpLeft2Msk, UpRight1Msk;
Pixmap	UpRight2Xbm, DownLeft1Xbm, DownLeft2Xbm, DownRight1Xbm, DownRight2Xbm;
Pixmap	UpRight2Msk, DownLeft1Msk, DownLeft2Msk, DownRight1Msk, DownRight2Msk;

Pixmap	UpTogi1Xbm, UpTogi2Xbm, DownTogi1Xbm, DownTogi2Xbm, LeftTogi1Xbm;
Pixmap	UpTogi1Msk, UpTogi2Msk, DownTogi1Msk, DownTogi2Msk, LeftTogi1Msk;
Pixmap	LeftTogi2Xbm, RightTogi1Xbm, RightTogi2Xbm;
Pixmap	LeftTogi2Msk, RightTogi1Msk, RightTogi2Msk;

GC      Mati2GC;

GC	Jare2GC, Kaki1GC, Kaki2GC, Mati3GC, Sleep1GC, Sleep2GC;

GC	AwakeGC;

GC	Up1GC, Up2GC, Down1GC, Down2GC, Left1GC, Left2GC, Right1GC, Right2GC;
GC	UpLeft1GC, UpLeft2GC, UpRight1GC, UpRight2GC, DownLeft1GC, DownLeft2GC;
GC	DownRight1GC, DownRight2GC;

GC	UpTogi1GC, UpTogi2GC, DownTogi1GC, DownTogi2GC, LeftTogi1GC;
GC	LeftTogi2GC, RightTogi1GC, RightTogi2GC;


typedef struct {
    GC		*GCCreatePtr;
    Pixmap	*BitmapCreatePtr;
    char	*PixelPattern;
    Pixmap	*BitmapMasksPtr;
    char	*MaskPattern;
} BitmapGCData;

BitmapGCData	BitmapGCDataTable[] =
{
    { &Mati2GC, &Mati2Xbm, mati2_bits, &Mati2Msk, mati2_mask_bits },
    { &Jare2GC, &Jare2Xbm, jare2_bits, &Jare2Msk, jare2_mask_bits },
    { &Kaki1GC, &Kaki1Xbm, kaki1_bits, &Kaki1Msk, kaki1_mask_bits },
    { &Kaki2GC, &Kaki2Xbm, kaki2_bits, &Kaki2Msk, kaki2_mask_bits },
    { &Mati3GC, &Mati3Xbm, mati3_bits, &Mati3Msk, mati3_mask_bits },
    { &Sleep1GC, &Sleep1Xbm, sleep1_bits, &Sleep1Msk, sleep1_mask_bits },
    { &Sleep2GC, &Sleep2Xbm, sleep2_bits, &Sleep2Msk, sleep2_mask_bits },
    { &AwakeGC, &AwakeXbm, awake_bits, &AwakeMsk, awake_mask_bits },
    { &Up1GC, &Up1Xbm, up1_bits, &Up1Msk, up1_mask_bits },
    { &Up2GC, &Up2Xbm, up2_bits, &Up2Msk, up2_mask_bits },
    { &Down1GC, &Down1Xbm, down1_bits, &Down1Msk, down1_mask_bits },
    { &Down2GC, &Down2Xbm, down2_bits, &Down2Msk, down2_mask_bits },
    { &Left1GC, &Left1Xbm, left1_bits, &Left1Msk, left1_mask_bits },
    { &Left2GC, &Left2Xbm, left2_bits, &Left2Msk, left2_mask_bits },
    { &Right1GC, &Right1Xbm, right1_bits, &Right1Msk, right1_mask_bits },
    { &Right2GC, &Right2Xbm, right2_bits, &Right2Msk, right2_mask_bits },
    { &UpLeft1GC, &UpLeft1Xbm, upleft1_bits, &UpLeft1Msk, upleft1_mask_bits },
    { &UpLeft2GC, &UpLeft2Xbm, upleft2_bits, &UpLeft2Msk, upleft2_mask_bits },
    { &UpRight1GC, &UpRight1Xbm, upright1_bits, &UpRight1Msk, upright1_mask_bits },
    { &UpRight2GC, &UpRight2Xbm, upright2_bits, &UpRight2Msk, upright2_mask_bits },
    { &DownLeft1GC, &DownLeft1Xbm, dwleft1_bits, &DownLeft1Msk, dwleft1_mask_bits },
    { &DownLeft2GC, &DownLeft2Xbm, dwleft2_bits, &DownLeft2Msk, dwleft2_mask_bits },
    { &DownRight1GC, &DownRight1Xbm, dwright1_bits, &DownRight1Msk, dwright1_mask_bits },
    { &DownRight2GC, &DownRight2Xbm, dwright2_bits, &DownRight2Msk, dwright2_mask_bits },
    { &UpTogi1GC, &UpTogi1Xbm, utogi1_bits, &UpTogi1Msk, utogi1_mask_bits },
    { &UpTogi2GC, &UpTogi2Xbm, utogi2_bits, &UpTogi2Msk, utogi2_mask_bits },
    { &DownTogi1GC, &DownTogi1Xbm, dtogi1_bits, &DownTogi1Msk, dtogi1_mask_bits },
    { &DownTogi2GC, &DownTogi2Xbm, dtogi2_bits, &DownTogi2Msk, dtogi2_mask_bits },
    { &LeftTogi1GC, &LeftTogi1Xbm, ltogi1_bits, &LeftTogi1Msk, ltogi1_mask_bits },
    { &LeftTogi2GC, &LeftTogi2Xbm, ltogi2_bits, &LeftTogi2Msk, ltogi2_mask_bits },
    { &RightTogi1GC, &RightTogi1Xbm, rtogi1_bits, &RightTogi1Msk, rtogi1_mask_bits },
    { &RightTogi2GC, &RightTogi2Xbm, rtogi2_bits, &RightTogi2Msk, rtogi2_mask_bits },
    { NULL, NULL, NULL, NULL, NULL }
};

typedef struct {
    GC		*TickGCPtr;
    Pixmap	*TickMaskPtr;
} Animation;

Animation	AnimationPattern[][2] =
{
  { { &Mati2GC, &Mati2Msk },
    { &Mati2GC, &Mati2Msk } },		/* NekoState == NEKO_STOP */
  { { &Jare2GC, &Jare2Msk },
    { &Mati2GC, &Mati2Msk } },		/* NekoState == NEKO_JARE */
  { { &Kaki1GC, &Kaki1Msk },
    { &Kaki2GC, &Kaki2Msk } },		/* NekoState == NEKO_KAKI */
  { { &Mati3GC, &Mati3Msk },
    { &Mati3GC, &Mati3Msk } },		/* NekoState == NEKO_AKUBI */
  { { &Sleep1GC, &Sleep1Msk },
    { &Sleep2GC, &Sleep2Msk } },		/* NekoState == NEKO_SLEEP */
  { { &AwakeGC, &AwakeMsk },
    { &AwakeGC, &AwakeMsk } },		/* NekoState == NEKO_AWAKE */
  { { &Up1GC, &Up1Msk },
    { &Up2GC, &Up2Msk } },		/* NekoState == NEKO_U_MOVE */
  { { &Down1GC, &Down1Msk },
    { &Down2GC, &Down2Msk } },		/* NekoState == NEKO_D_MOVE */
  { { &Left1GC, &Left1Msk },
    { &Left2GC, &Left2Msk } },		/* NekoState == NEKO_L_MOVE */
  { { &Right1GC, &Right1Msk },
    { &Right2GC, &Right2Msk } },		/* NekoState == NEKO_R_MOVE */
  { { &UpLeft1GC, &UpLeft1Msk },
    { &UpLeft2GC, &UpLeft2Msk } },	/* NekoState == NEKO_UL_MOVE */
  { { &UpRight1GC, &UpRight1Msk },
    { &UpRight2GC, &UpRight2Msk } },	/* NekoState == NEKO_UR_MOVE */
  { { &DownLeft1GC, &DownLeft1Msk },
    { &DownLeft2GC, &DownLeft2Msk } },	/* NekoState == NEKO_DL_MOVE */
  { { &DownRight1GC, &DownRight1Msk },
    { &DownRight2GC, &DownRight2Msk } },	/* NekoState == NEKO_DR_MOVE */
  { { &UpTogi1GC, &UpTogi1Msk },
    { &UpTogi2GC, &UpTogi2Msk } },	/* NekoState == NEKO_U_TOGI */
  { { &DownTogi1GC, &DownTogi1Msk },
    { &DownTogi2GC, &DownTogi2Msk } },	/* NekoState == NEKO_D_TOGI */
  { { &LeftTogi1GC, &LeftTogi1Msk },
    { &LeftTogi2GC, &LeftTogi2Msk } },	/* NekoState == NEKO_L_TOGI */
  { { &RightTogi1GC, &RightTogi1Msk },
    { &RightTogi2GC, &RightTogi2Msk } },	/* NekoState == NEKO_R_TOGI */
};

static void NullFunction(int signum);
static int ParseLongOption(const char *option, const char *value,
                           long minimum, long *result);
static int ParseDoubleOption(const char *option, const char *value,
                             double minimum, double *result);
static void PlaceBedOnMonitor(void);

/*
 *	$@%S%C%H%^%C%W%G!<%?!&(JGC $@=i4|/*
 *	$@%9%/%j!<%s4D6-=i4|2=(J
 */

void
InitBitmapAndGCs()
{
    BitmapGCData	*BitmapGCDataTablePtr;
    XGCValues		theGCValues;

    theGCValues.function = GXcopy;
    theGCValues.foreground = theForegroundColor.pixel;
    theGCValues.background = theBackgroundColor.pixel;
    theGCValues.fill_style = FillTiled;
    theGCValues.ts_x_origin = 0;
    theGCValues.ts_y_origin = 0;

    for (BitmapGCDataTablePtr = BitmapGCDataTable;
	 BitmapGCDataTablePtr->GCCreatePtr != NULL;
	 BitmapGCDataTablePtr++) {

	*(BitmapGCDataTablePtr->BitmapCreatePtr)
	    = XCreatePixmapFromBitmapData(theDisplay, theRoot,
		BitmapGCDataTablePtr->PixelPattern,
		BITMAP_WIDTH, BITMAP_HEIGHT,
		theForegroundColor.pixel,
		theBackgroundColor.pixel,
		DefaultDepth(theDisplay, theScreen));

	theGCValues.tile = *(BitmapGCDataTablePtr->BitmapCreatePtr);

	*(BitmapGCDataTablePtr->BitmapMasksPtr)
	    = XCreateBitmapFromData(theDisplay, theRoot,
		BitmapGCDataTablePtr->MaskPattern,
		BITMAP_WIDTH, BITMAP_HEIGHT);

	*(BitmapGCDataTablePtr->GCCreatePtr)
	    = XCreateGC(theDisplay, theWindow,
		GCFunction | GCForeground | GCBackground | GCTile |
		GCTileStipXOrigin | GCTileStipYOrigin | GCFillStyle,
		&theGCValues);
    }
}

/*
 *	$@%j%=!<%9!&%G!<%?%Y!<%9$+$iI,MW$J%j%=!<%9$r<h$j=P$9(J
 */

char *
NekoGetDefault(char *resource)
{
	char	*value;

	if ((value = XGetDefault(theDisplay, ProgramName, resource)) != NULL) {
		return value;
	}
	if ((value = XGetDefault(theDisplay, ClassName, resource)) != NULL) {
		return value;
	}
	return NULL;
}

/*
 *	$@%j%=!<%9!&%G!<%?%Y!<%9$+$i%*%W%7%g%s$r@_Dj(J
 */

void
GetResources(void)
{
  char	*resource;
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

  if (NekoSpeed == (double)0) {
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
  if (NekoSpeed == (double)0) {
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
    XColor	theExactColor;
    Colormap	theColormap;

    theColormap = DefaultColormap(theDisplay, theScreen);

    if (theDepth == 1) {
	Foreground = "black";
	Background = "white";
    }

    if (ReverseVideo == True) {
	char	*tmp;

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

/*
 *	$@%9%/%j!<%s4D6-=i4|2=(J
 */

void
InitMonitors()
{
    Window root;

    root = RootWindow(
        theDisplay,
        DefaultScreen(theDisplay)
    );

    Monitors =
        XRRGetMonitors(
            theDisplay,
            root,
            True,
            &MonitorCount
        );
}

static void
PlaceBedOnMonitor(void)
{
  int monitor;
  int max_x;
  int max_y;

  if (MonitorCount <= 0) {
    BedX = ((int)WindowWidth - bed_width) / 2;
    BedY = ((int)WindowHeight - bed_height) / 2;
    return;
  }

  monitor = RestrictMonitor >= 0 ? RestrictMonitor : rand() % MonitorCount;
  max_x = Monitors[monitor].width - bed_width;
  max_y = Monitors[monitor].height - bed_height;
  BedX = Monitors[monitor].x + (max_x > 0 ? rand() % (max_x + 1) : 0);
  BedY = Monitors[monitor].y + (max_y > 0 ? rand() % (max_y + 1) : 0);
}

void
InitScreen(char *DisplayName)
{
  XSetWindowAttributes	theWindowAttributes;
  unsigned long		theWindowMask;
  Window			theTempRoot;
  int				WindowPointX;
  int				WindowPointY;
  unsigned int		BorderWidth;
  int				event_base, error_base;

  if ((theDisplay = XOpenDisplay(DisplayName)) == NULL) {
    fprintf(stderr, "%s: Can't open display", ProgramName);
    if (DisplayName != NULL) {
      fprintf(stderr, " %s.\n", DisplayName);
    } else {
      fprintf(stderr, ".\n");
    }
    exit(1);
  }

  InitMonitors();

  if (RestrictMonitor >= MonitorCount) {
    fprintf(stderr, "%s: monitor %d is unavailable.\n", ProgramName,
            RestrictMonitor);
    exit(1);
  }

  GetResources();

  if (Synchronous == True) {
    fprintf(stderr,"Synchronizing.\n");
    XSynchronize(theDisplay,True);
  }

#ifdef SHAPE
  if (!NoShape && XShapeQueryExtension(theDisplay,
				       &event_base, &error_base) == False) {
    fprintf(stderr, "Display not suported shape extension.\n");
    NoShape = True;
				       }
#endif /* SHAPE */

  theScreen = DefaultScreen(theDisplay);
  theDepth = DefaultDepth(theDisplay, theScreen);

  theRoot = RootWindow(theDisplay, theScreen);

  XGetGeometry(theDisplay, theRoot, &theTempRoot,
	       &WindowPointX, &WindowPointY,
	       &WindowWidth, &WindowHeight,
	       &BorderWidth, &theDepth);

  if (UseBed) {
    PlaceBedOnMonitor();
  }

  SetupColors();

  theWindowAttributes.background_pixel = theBackgroundColor.pixel;
  theWindowAttributes.override_redirect = True;

  theWindowMask = CWBackPixel		|
      CWOverrideRedirect;

  theWindow = XCreateWindow(theDisplay, theRoot, 0, 0,
			    BITMAP_WIDTH, BITMAP_HEIGHT,
			    0, theDepth, InputOutput, CopyFromParent,
			    theWindowMask, &theWindowAttributes);
  InitBitmapAndGCs();

  XSelectInput(theDisplay, theWindow, 
	       ExposureMask|VisibilityChangeMask|KeyPressMask);

  if (UseBed) {
    BedWindow = XCreateWindow(theDisplay, theRoot, BedX, BedY,
            bed_width, bed_height,
            0, theDepth, InputOutput, CopyFromParent,
            theWindowMask, &theWindowAttributes);
    BedPixmap = XCreateBitmapFromData(theDisplay, theRoot, bed_bits,
            bed_width, bed_height);
    BedMask = XCreateBitmapFromData(theDisplay, theRoot, bed_mask_bits,
            bed_mask_width, bed_mask_height);
#ifdef SHAPE
    if (!NoShape) {
      XShapeCombineMask(theDisplay, BedWindow, ShapeBounding, 0, 0,
              BedMask, ShapeSet);
    }
#endif
    BedGC = XCreateGC(theDisplay, theRoot, 0, NULL);
    XSetForeground(theDisplay, BedGC, theForegroundColor.pixel);
    XSetBackground(theDisplay, BedGC, theBackgroundColor.pixel);
    XSetStipple(theDisplay, BedGC, BedPixmap);
    XSetFillStyle(theDisplay, BedGC, FillOpaqueStippled);
    XSelectInput(theDisplay, BedWindow,
            ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    XMapWindow(theDisplay, BedWindow);
    XFillRectangle(theDisplay, BedWindow, BedGC, 0, 0, bed_width, bed_height);
  }

  XFlush(theDisplay);
}


/*
 *	SIGINT $@%7%0%J%k=hM}(J
 */

void
RestoreCursor()
{
  XSetWindowAttributes	theWindowAttributes;
  BitmapGCData *BitmapGCDataTablePtr;

  theWindowAttributes.cursor = None;
  XChangeWindowAttributes(theDisplay, theRoot, CWCursor,
			  &theWindowAttributes);
  for (BitmapGCDataTablePtr = BitmapGCDataTable;
       BitmapGCDataTablePtr->GCCreatePtr != NULL;
       BitmapGCDataTablePtr++) {
    XFreePixmap(theDisplay,*(BitmapGCDataTablePtr->BitmapCreatePtr));
    XFreePixmap(theDisplay,*(BitmapGCDataTablePtr->BitmapMasksPtr));
    XFreeGC(theDisplay,*(BitmapGCDataTablePtr->GCCreatePtr));
       }

  if (UseBed) {
    XFreePixmap(theDisplay, BedPixmap);
    XFreePixmap(theDisplay, BedMask);
    XFreeGC(theDisplay, BedGC);
  }

  if (Monitors)
  {
    XRRFreeMonitors(Monitors);
  }

  XCloseDisplay(theDisplay);
  exit(0);
}

void
RestoreCursorHandler(int signum)
{
  (void)signum;
  TerminationRequested = 1;
}

/*
 *	$@%$%s%?!<%P%k(J
 *
 *	$@!!$3$N4X?t$r8F$V$H!"$"$k0lDj$N;~4VJV$C$F$3$J$/$J$k!#G-(J
 *	$@$NF0:n%?%$%_%s%0D4@0$KMxMQ$9$k$3$H!#(J
 */

void
Interval()
{
    pause();
    if (RaiseWindowDelay > 0)
      RaiseWindowDelay--;
}


/*
 *	$@%F%#%C%/%+%&%s%H=hM}(J
 */

void
TickCount()
{
    if (++NekoTickCount >= MAX_TICK) {
	NekoTickCount = 0;
    }

    if (NekoTickCount % 2 == 0) {
	if (NekoStateCount < MAX_TICK) {
	    NekoStateCount++;
	}
    }
}


/*
 *	$@G->uBV@_Dj(J
 */

void
SetNekoState(int SetValue)
{
    NekoTickCount = 0;
    NekoStateCount = 0;

    NekoState = SetValue;
}


/*
 *	$@G-IA2h=hM}(J
 */

void
DrawNeko(int x, int y, Animation DrawAnime)
{
/*@@@@@@*/
    register GC		DrawGC = *(DrawAnime.TickGCPtr);
    register Pixmap	DrawMask = *(DrawAnime.TickMaskPtr);

    if ((x != NekoLastX) || (y != NekoLastY)
		|| (DrawGC != NekoLastGC)) {
      XWindowChanges	theChanges;

      theChanges.x = x;
      theChanges.y = y;
      XConfigureWindow(theDisplay, theWindow, CWX | CWY, &theChanges);
#ifdef SHAPE
      if (NoShape == False) {
	XShapeCombineMask(theDisplay, theWindow, ShapeBounding,
			  0, 0, DrawMask, ShapeSet);

      }
#endif /* SHAPE */
      if (DontMapped) {
	XMapWindow(theDisplay, theWindow);
	DontMapped = 0;
      }
      XFillRectangle(theDisplay, theWindow, DrawGC,
		     0, 0, BITMAP_WIDTH, BITMAP_HEIGHT);
    }

    XFlush(theDisplay);

    NekoLastX = x;
    NekoLastY = y;

    NekoLastGC = DrawGC;
}


/*
 *	$@G-:FIA2h=hM}(J
 */

void
RedrawNeko()
{
  XFillRectangle(theDisplay, theWindow, NekoLastGC,
		 0, 0, BITMAP_WIDTH, BITMAP_HEIGHT);

  XFlush(theDisplay);
}


/*
 *	$@G-0\F0J}K!7hDj(J
 *
 *      This sets the direction that the neko is moving.
 *
 */

void
NekoDirection()
{
    int			NewState;
    double		LargeX, LargeY;
    double		Length;
    double		SinTheta;

    if (NekoMoveDx == 0 && NekoMoveDy == 0) {
	NewState = NEKO_STOP;
    } else {
	LargeX = (double)NekoMoveDx;
	LargeY = (double)(-NekoMoveDy);
	Length = sqrt(LargeX * LargeX + LargeY * LargeY);
	SinTheta = LargeY / Length;

	if (NekoMoveDx > 0) {
	    if (SinTheta > SinPiPer8Times3) {
		NewState = NEKO_U_MOVE;
	    } else if ((SinTheta <= SinPiPer8Times3)
			&& (SinTheta > SinPiPer8)) {
		NewState = NEKO_UR_MOVE;
	    } else if ((SinTheta <= SinPiPer8)
			&& (SinTheta > -(SinPiPer8))) {
		NewState = NEKO_R_MOVE;
	    } else if ((SinTheta <= -(SinPiPer8))
			&& (SinTheta > -(SinPiPer8Times3))) {
		NewState = NEKO_DR_MOVE;
	    } else {
		NewState = NEKO_D_MOVE;
	    }
	} else {
	    if (SinTheta > SinPiPer8Times3) {
		NewState = NEKO_U_MOVE;
	    } else if ((SinTheta <= SinPiPer8Times3)
			&& (SinTheta > SinPiPer8)) {
		NewState = NEKO_UL_MOVE;
	    } else if ((SinTheta <= SinPiPer8)
			&& (SinTheta > -(SinPiPer8))) {
		NewState = NEKO_L_MOVE;
	    } else if ((SinTheta <= -(SinPiPer8))
			&& (SinTheta > -(SinPiPer8Times3))) {
		NewState = NEKO_DL_MOVE;
	    } else {
		NewState = NEKO_D_MOVE;
	    }
	}
    }

    if (NekoState != NewState) {
	SetNekoState(NewState);
    }
}


/*
 *	$@G-JI$V$D$+$jH=Dj(J
 */

Bool
IsWindowOver()
{
    Bool	ReturnValue = False;

    if (NekoY <= 0) {
	NekoY = 0;
	ReturnValue = True;
    } else if (NekoY >= WindowHeight - BITMAP_HEIGHT) {
	NekoY = WindowHeight - BITMAP_HEIGHT;
	ReturnValue = True;
    }
    if (NekoX <= 0) {
	NekoX = 0;
	ReturnValue = True;
    } else if (NekoX >= WindowWidth - BITMAP_WIDTH) {
	NekoX = WindowWidth - BITMAP_WIDTH;
	ReturnValue = True;
    }

    return(ReturnValue);
}

/*
 *	$@G-0\F0>u67H=Dj(J
 */

Bool
IsNekoDontMove()
{
    return NekoX == NekoLastX && NekoY == NekoLastY;
}


/*
 *	$@G-0\F03+;OH=Dj(J
 */

Bool
IsNekoMoveStart()
{
    return NekoMoveDx != 0 || NekoMoveDy != 0;
}


/*
 *	$@G-0\F0(J dx, dy $@7W;;(J
 */

void
PickRandomTarget()
{
    int m;

    if (MonitorCount <= 0) {
        TargetX = rand() % WindowWidth;
        TargetY = rand() % WindowHeight;
        return;
    }

    if (RestrictMonitor >= 0 &&
      RestrictMonitor < MonitorCount)
    {
      m = RestrictMonitor;
    }
    else
    {
      m = rand() % MonitorCount;
    }

    int width =
        Monitors[m].width > BITMAP_WIDTH ?
        Monitors[m].width - BITMAP_WIDTH : 1;

    int height =
        Monitors[m].height > BITMAP_HEIGHT ?
        Monitors[m].height - BITMAP_HEIGHT : 1;

    TargetX =
        Monitors[m].x +
        rand() % (width + 1);

    TargetY =
        Monitors[m].y +
        rand() % (height + 1);
}

int
RectOnMonitor(int x, int y, int w, int h)
{
    int i;

    if (MonitorCount <= 0) {
        return x >= 0 && y >= 0 &&
            x + w <= (int)WindowWidth && y + h <= (int)WindowHeight;
    }

    if (RestrictMonitor >= 0 &&
        RestrictMonitor < MonitorCount)
    {
        i = RestrictMonitor;

        return
            x >= Monitors[i].x &&
            y >= Monitors[i].y &&
            x + w <= Monitors[i].x + Monitors[i].width &&
            y + h <= Monitors[i].y + Monitors[i].height;
    }

    for (i = 0; i < MonitorCount; i++)
    {
        if (x >= Monitors[i].x &&
            y >= Monitors[i].y &&
            x + w <= Monitors[i].x + Monitors[i].width &&
            y + h <= Monitors[i].y + Monitors[i].height)
        {
            return 1;
        }
    }

    return 0;
}

void
PickNearbyTarget(int radius)
{
    int x, y;

    for (int tries = 0; tries < 100; tries++)
    {
        x = NekoX + (rand() % (radius * 2 + 1) - radius);
        y = NekoY + (rand() % (radius * 2 + 1) - radius);

        if (RectOnMonitor(
            x,
            y,
            BITMAP_WIDTH,
            BITMAP_HEIGHT))
        {
            TargetX = x;
            TargetY = y;
            return;
        }
    }

    PickRandomTarget();
}

void
CalcDxDy()
{
    double LargeX, LargeY;
    double DoubleLength, Length;

    if (Zoomies && time(NULL) > ZoomiesEndTime) {
        Zoomies = 0;
    }

    if (Waiting) {

        if (time(NULL) < NextMoveTime) {
            NekoMoveDx = 0;
            NekoMoveDy = 0;
            return;
        }

        Waiting = 0;

        GoingToBed = 0;

        if (Zoomies) {
            PickRandomTarget();
        } else {
            int r = rand() % 100;

            if (UseBed && r < 15)
            {
                TargetX = BedX + 16;
                TargetY = BedY + 32;
                GoingToBed = 1;
            }
            else if (r < (UseBed ? 55 : 40))
            {
                PickNearbyTarget(300);
            }
            else if (r < (UseBed ? 85 : 70))
            {
                PickRandomTarget();
            }
            else
            {
                TargetX = NekoX;
                TargetY = NekoY;
            }
        }

        if (!Zoomies && rand() % 100 < 5) {
            Zoomies = 1;
            ZoomiesEndTime = time(NULL) + 50;
        }

    }

    int ReachDistance = 2;

    if (!Waiting &&
        abs(TargetX - NekoX - BITMAP_WIDTH / 2) < ReachDistance &&
        abs(TargetY - NekoY - BITMAP_HEIGHT) < ReachDistance)
    {

        int delay;

        Waiting = 1;

        delay =
            (int)((MinWait +
            rand() % (MaxWait - MinWait + 1)));

        if (GoingToBed)
        {
            delay *= 2;
        }

        if (Zoomies)
            delay = 1 + rand() % 3;

        NextMoveTime = time(NULL) + delay;

        NekoMoveDx = 0;
        NekoMoveDy = 0;
        return;
    }

    LargeX = (double)(TargetX - NekoX - BITMAP_WIDTH / 2);
    LargeY = (double)(TargetY - NekoY - BITMAP_HEIGHT);

    DoubleLength = LargeX * LargeX + LargeY * LargeY;

    if (DoubleLength != 0.0) {
        Length = sqrt(DoubleLength);

        if (Length <= NekoSpeed) {
            NekoMoveDx = (int)LargeX;
            NekoMoveDy = (int)LargeY;
        } else {
            NekoMoveDx = (int)((NekoSpeed * LargeX) / Length);
            NekoMoveDy = (int)((NekoSpeed * LargeY) / Length);
        }
    } else {
        NekoMoveDx = 0;
        NekoMoveDy = 0;
    }
}

/*
 *	$@F0:n2r@OG-IA2h=hM}(J
 */

void
NekoThinkDraw()
{
    CalcDxDy();

    if (NekoState != NEKO_SLEEP) {
	DrawNeko(NekoX, NekoY,
		AnimationPattern[NekoState][NekoTickCount & 0x1]);
    } else {
	DrawNeko(NekoX, NekoY,
		AnimationPattern[NekoState][(NekoTickCount >> 2) & 0x1]);
    }

    TickCount();

    switch (NekoState) {
    case NEKO_STOP:
	if (IsNekoMoveStart()) {
	    SetNekoState(NEKO_AWAKE);
	    break;
	}
	if (NekoStateCount < NEKO_STOP_TIME) {
	    break;
	}
	if (NekoMoveDx < 0 && NekoX <= 0) {
	    SetNekoState(NEKO_L_TOGI);
	} else if (NekoMoveDx > 0 && NekoX >= WindowWidth - BITMAP_WIDTH) {
	    SetNekoState(NEKO_R_TOGI);
	} else if ((NekoMoveDy < 0 && NekoY <= 0)
		  ){
	    SetNekoState(NEKO_U_TOGI);
	} else if ((NekoMoveDy > 0 && NekoY >= WindowHeight - BITMAP_HEIGHT)
		  ){
	    SetNekoState(NEKO_D_TOGI);
	} else {
	    SetNekoState(NEKO_JARE);
	}
	break;
    case NEKO_JARE:
	if (IsNekoMoveStart()) {
	    SetNekoState(NEKO_AWAKE);
	    break;
	}
	if (NekoStateCount < NEKO_JARE_TIME) {
	    break;
	}
	SetNekoState(NEKO_KAKI);
	break;
    case NEKO_KAKI:
	if (IsNekoMoveStart()) {
	    SetNekoState(NEKO_AWAKE);
	    break;
	}
	if (NekoStateCount < NEKO_KAKI_TIME) {
	    break;
	}
	SetNekoState(NEKO_AKUBI);
	break;
    case NEKO_AKUBI:
	if (IsNekoMoveStart()) {
	    SetNekoState(NEKO_AWAKE);
	    break;
	}
	if (NekoStateCount < NEKO_AKUBI_TIME) {
	    break;
	}
	SetNekoState(NEKO_SLEEP);
	break;
    case NEKO_SLEEP:
	if (IsNekoMoveStart()) {
	    SetNekoState(NEKO_AWAKE);
	    break;
	}
	break;
    case NEKO_AWAKE:
	if (NekoStateCount < NEKO_AWAKE_TIME) {
	    break;
	}
	NekoDirection();	/* $@G-$,F0$/8~$-$r5a$a$k(J */
	break;
    case NEKO_U_MOVE:
    case NEKO_D_MOVE:
    case NEKO_L_MOVE:
    case NEKO_R_MOVE:
    case NEKO_UL_MOVE:
    case NEKO_UR_MOVE:
    case NEKO_DL_MOVE:
    case NEKO_DR_MOVE:
	NekoX += NekoMoveDx;
	NekoY += NekoMoveDy;
	NekoDirection();
	if (IsWindowOver()) {
	    if (IsNekoDontMove()) {
		SetNekoState(NEKO_STOP);
	    }
	}
	break;
    case NEKO_U_TOGI:
    case NEKO_D_TOGI:
    case NEKO_L_TOGI:
    case NEKO_R_TOGI:
	if (IsNekoMoveStart()) {
	    SetNekoState(NEKO_AWAKE);
	    break;
	}
	if (NekoStateCount < NEKO_TOGI_TIME) {
	    break;
	}
	SetNekoState(NEKO_KAKI);
	break;
    default:
	/* Internal Error */
	SetNekoState(NEKO_STOP);
	break;
    }

    Interval();
}


/*
 *	$@%-!<%$%Y%s%H=hM}(J
 */

Bool
ProcessKeyPress(XKeyEvent *theKeyEvent)
{
  int			Length;
  int			theKeyBufferMaxLen = AVAIL_KEYBUF;
  char		theKeyBuffer[AVAIL_KEYBUF + 1];
  KeySym		theKeySym;
  XComposeStatus	theComposeStatus;
  Bool		ReturnState;

  ReturnState = True;

  Length = XLookupString(theKeyEvent,
			 theKeyBuffer, theKeyBufferMaxLen,
			 &theKeySym, &theComposeStatus);

  if (Length > 0) {
    switch (theKeyBuffer[0]) {
    case 'q':
    case 'Q':
      if (theKeyEvent->state & Mod1Mask) { /* META (Alt) $@%-!<(J */
	ReturnState = False;
      }
      break;
    default:
      break;
    }
  }

  return(ReturnState);
}


/*
 *	$@%$%Y%s%H=hM}(J
 */

Bool
ProcessEvent()
{
    XEvent	theEvent;
    Bool	ContinueState = True;

    while (XPending(theDisplay)) {
        XNextEvent(theDisplay,&theEvent);
	switch (theEvent.type) {
	case Expose:
	    if (theEvent.xexpose.count == 0) {
		if (UseBed && theEvent.xexpose.window == BedWindow) {
		    XFillRectangle(theDisplay, BedWindow, BedGC, 0, 0,
			    bed_width, bed_height);
		} else if (theEvent.xexpose.window == theWindow) {
		    RedrawNeko();
		}
	    }
	    break;
	case KeyPress:
	    ContinueState = ProcessKeyPress(&theEvent.xkey);
	    if (!ContinueState) {
		    return(ContinueState);
	    }
	    break;
	case VisibilityNotify:
	    if (RaiseWindowDelay==0) {
	      XRaiseWindow(theDisplay,theWindow);
	      RaiseWindowDelay=DEFAULT_RAISE_WAIT;
	    }
	    break;
  case ButtonPress:
    if (UseBed && theEvent.xbutton.window == BedWindow &&
        theEvent.xbutton.button == Button1)
    {
        if (XGrabPointer(theDisplay, BedWindow, False,
                ButtonReleaseMask | PointerMotionMask,
                GrabModeAsync, GrabModeAsync, None, None,
                theEvent.xbutton.time) == GrabSuccess) {
            DraggingBed = 1;
            DragOffsetX = theEvent.xbutton.x;
            DragOffsetY = theEvent.xbutton.y;
        }
    }
    break;
  case MotionNotify:
  if (UseBed && DraggingBed)
    {
    int NewX =
        theEvent.xmotion.x_root - DragOffsetX;

    int NewY =
        theEvent.xmotion.y_root - DragOffsetY;

    if (RectOnMonitor(
        NewX,
        NewY,
        bed_width,
        bed_height))
    {
        BedX = NewX;
        BedY = NewY;

        XMoveWindow(
            theDisplay,
            BedWindow,
            BedX,
            BedY
        );
    }
  }
    break;
  case ButtonRelease:
    if (UseBed && DraggingBed && theEvent.xbutton.button == Button1)
    {
        DraggingBed = 0;
        XUngrabPointer(theDisplay, theEvent.xbutton.time);
    }
    break;
	default:
	    /* Unknown Event */
	    break;
	}
    }

    return(ContinueState);
}


/*
 *	$@G-=hM}(J
 */

void
ProcessNeko()
{
  struct itimerval	Value;

  /* $@G-$N=i4|2=(J */

  PickRandomTarget();

  NekoX = TargetX;
  NekoY = TargetY;

  NekoLastX = NekoX;
  NekoLastY = NekoY;

  Waiting = 1;
  NextMoveTime = time(NULL);

  SetNekoState(NEKO_STOP);

  /* $@%?%$%^!<@_Dj(J */

  timerclear(&Value.it_interval);
  timerclear(&Value.it_value);

  Value.it_interval.tv_sec = IntervalTime / 1000000L;
  Value.it_interval.tv_usec = IntervalTime % 1000000L;
  Value.it_value.tv_sec = Value.it_interval.tv_sec;
  Value.it_value.tv_usec = Value.it_interval.tv_usec;

  if (setitimer(ITIMER_REAL, &Value, 0) != 0) {
    perror("setitimer");
    return;
  }

  /* $@%a%$%s=hM}(J */

  do {
    NekoThinkDraw();
  } while (!TerminationRequested && ProcessEvent());
}


/*
 *	SIGALRM $@%7%0%J%k=hM}(J
 */

static void
NullFunction(int signum)
{
  (void)signum;
  /* No Operation */
#if defined(SYSV) || defined(SVR4)
  signal(SIGALRM, NullFunction);
#endif /* SYSV || SVR4 */
}

/*
 *	$@%(%i!<=hM}(J
 */

int
NekoErrorHandler(Display *dpy, XErrorEvent *err)
{
    char msg[80];
    XGetErrorText(dpy, err->error_code, msg, 80);
    fprintf(stderr, "%s: Error and exit.\n%s\n", ProgramName, msg);
    exit(1);
}


static int
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

static int
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

/*
 *	Usage
 */

char	*message[] = {
"",
"Options are:",
"--monitor <number>	: neko appears on specified monitor",
"--fg <color>		: foreground color",
"--bg <color>		: background color",
"--speed <dots>",
"--time <microseconds>",
"--min-wait <seconds>	: set minimum idle time between walks (default: 20)",
"--max-wait <seconds>	: set maximum idle time between walks (default: 150)",
"--bed\t\t\t: show a draggable bed",
NULL };

void
Usage()
{
  char	**mptr;

  mptr = message;
  fprintf(stderr, "Usage: %s [<options>]\n", ProgramName);
  while (*mptr) {
    fprintf(stderr,"%s\n", *mptr);
    mptr++;
  }
}


/*
 *	$@%*%W%7%g%s$NM}2r(J
 */

void
GetArguments(int argc, char *argv[], char *theDisplayName)
{
  int		ArgCounter;
  long value;

  theDisplayName[0] = '\0';

  for (ArgCounter = 0; ArgCounter < argc; ArgCounter++) {

    if (strcmp(argv[ArgCounter], "--help") == 0 ||
        strcmp(argv[ArgCounter], "-h") == 0)
    {
        Usage();
        exit(0);
    }
    else if (strcmp(argv[ArgCounter], "--monitor") == 0)
    {
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
    }
    else if (strcmp(argv[ArgCounter], "--speed") == 0) {
      ArgCounter++;
      if (ArgCounter < argc) {
	if (!ParseDoubleOption("--speed", argv[ArgCounter], 0.0, &NekoSpeed)) {
	  exit(1);
	}
      } else {
	fprintf(stderr, "%s: --speed option error.\n", ProgramName);
	exit(1);
      }
    }
    else if (strcmp(argv[ArgCounter], "--time") == 0) {
      ArgCounter++;
      if (ArgCounter < argc) {
	if (!ParseLongOption("--time", argv[ArgCounter], 1, &IntervalTime)) {
	  exit(1);
	}
      } else {
	fprintf(stderr, "%s: --time option error.\n", ProgramName);
	exit(1);
      }
    }
    else if ((strcmp(argv[ArgCounter], "--fg") == 0) ||
	     (strcmp(argv[ArgCounter], "--foreground") == 0)) {
      ArgCounter++;
      if (ArgCounter >= argc) {
	fprintf(stderr, "%s: --fg option error.\n", ProgramName);
	exit(1);
      }
      Foreground = argv[ArgCounter];
	     }
    else if ((strcmp(argv[ArgCounter], "--bg") == 0) ||
	     (strcmp(argv[ArgCounter], "--background") == 0)) {
      ArgCounter++;
      if (ArgCounter >= argc) {
	fprintf(stderr, "%s: --bg option error.\n", ProgramName);
	exit(1);
      }
      Background = argv[ArgCounter];
	     }
    else if (strcmp(argv[ArgCounter], "--noshape") == 0) {
      NoShape = True;
    }
    else if (strcmp(argv[ArgCounter], "--bed") == 0) {
      UseBed = True;
    }
    else if (strcmp(argv[ArgCounter], "--min-wait") == 0) {
      ArgCounter++;
      if (ArgCounter < argc) {
	if (!ParseLongOption("--min-wait", argv[ArgCounter], 0, &value) ||
	    value > INT_MAX) {
	  fprintf(stderr, "%s: --min-wait value is invalid.\n", ProgramName);
	  exit(1);
	}
        MinWait = (int)value;
      } else {
        fprintf(stderr, "%s: --min-wait option error.\n", ProgramName);
        exit(1);
      }
    }
    else if (strcmp(argv[ArgCounter], "--max-wait") == 0) {
      ArgCounter++;
      if (ArgCounter < argc) {
	if (!ParseLongOption("--max-wait", argv[ArgCounter], 0, &value) ||
	    value > INT_MAX) {
	  fprintf(stderr, "%s: --max-wait value is invalid.\n", ProgramName);
	  exit(1);
	}
        MaxWait = (int)value;
      } else {
        fprintf(stderr, "%s: --max-wait option error.\n", ProgramName);
        exit(1);
      }
    }
    else {
      fprintf(stderr, "%s: Unknown option \"%s\".\n", ProgramName,
              argv[ArgCounter]);
      Usage();
      exit(1);
    }
  }

  if (MaxWait < MinWait)
  {
    fprintf(stderr,
        "%s: max-wait must be >= min-wait.\n",
        ProgramName);
    exit(1);
  }

  if (strlen(theDisplayName) < 1) {
    theDisplayName[0] = '\0';
  }
}


/*
 *	$@%a%$%s4X?t(J
 */

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

  SinPiPer8Times3 = sin(PI_PER8 * (double)3);
  SinPiPer8 = sin(PI_PER8);

  ProcessNeko();

  RestoreCursor();
}
