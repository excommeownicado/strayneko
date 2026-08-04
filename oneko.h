#ifndef ONEKO_H
#define ONEKO_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <X11/extensions/Xrandr.h>

#include "bitmaps/neko/neko.include"
#include "bitmaps/bed/bed.include"
#include "bitmasks/neko/neko.mask.include"
#include "bitmasks/bed/bed.mask.include"

#define BITMAP_WIDTH 32
#define BITMAP_HEIGHT 32

#define AVAIL_KEYBUF 255
#define MAX_TICK 9999
#define DEFAULT_RAISE_WAIT 16

#define DEFAULT_FOREGROUND "black"
#define DEFAULT_BACKGROUND "white"

#define NEKO_STOP 0
#define NEKO_JARE 1
#define NEKO_KAKI 2
#define NEKO_AKUBI 3
#define NEKO_SLEEP 4
#define NEKO_AWAKE 5
#define NEKO_U_MOVE 6
#define NEKO_D_MOVE 7
#define NEKO_L_MOVE 8
#define NEKO_R_MOVE 9
#define NEKO_UL_MOVE 10
#define NEKO_UR_MOVE 11
#define NEKO_DL_MOVE 12
#define NEKO_DR_MOVE 13
#define NEKO_U_TOGI 14
#define NEKO_D_TOGI 15
#define NEKO_L_TOGI 16
#define NEKO_R_TOGI 17

#define NEKO_STOP_TIME 4
#define NEKO_JARE_TIME 10
#define NEKO_KAKI_TIME 4
#define NEKO_AKUBI_TIME 6
#define NEKO_AWAKE_TIME 3
#define NEKO_TOGI_TIME 10

#define PI_PER8 ((double)3.1415926535/(double)8)
#define MAXDISPLAYNAME (64 + 5)
#define NOTDEFINED (-1)

#define IsTrue(str) (strcmp((str), "true") == 0)

typedef struct {
    GC *GCCreatePtr;
    Pixmap *BitmapCreatePtr;
    char *PixelPattern;
    Pixmap *BitmapMasksPtr;
    char *MaskPattern;
} BitmapGCData;

typedef struct {
    GC *TickGCPtr;
    Pixmap *TickMaskPtr;
} Animation;

extern char *ClassName;
extern char *ProgramName;

extern Display *theDisplay;
extern int theScreen;
extern unsigned int theDepth;
extern Window theRoot;
extern Window theWindow;
extern Window BedWindow;

extern GC BedGC;
extern Pixmap BedPixmap;
extern Pixmap BedMask;

extern Pixmap Mati2Xbm, Jare2Xbm, Kaki1Xbm, Kaki2Xbm, Mati3Xbm, Sleep1Xbm, Sleep2Xbm;
extern Pixmap Mati2Msk, Jare2Msk, Kaki1Msk, Kaki2Msk, Mati3Msk, Sleep1Msk, Sleep2Msk;
extern Pixmap AwakeXbm, AwakeMsk;
extern Pixmap Up1Xbm, Up2Xbm, Down1Xbm, Down2Xbm, Left1Xbm, Left2Xbm;
extern Pixmap Up1Msk, Up2Msk, Down1Msk, Down2Msk, Left1Msk, Left2Msk;
extern Pixmap Right1Xbm, Right2Xbm, UpLeft1Xbm, UpLeft2Xbm, UpRight1Xbm;
extern Pixmap Right1Msk, Right2Msk, UpLeft1Msk, UpLeft2Msk, UpRight1Msk;
extern Pixmap UpRight2Xbm, DownLeft1Xbm, DownLeft2Xbm, DownRight1Xbm, DownRight2Xbm;
extern Pixmap UpRight2Msk, DownLeft1Msk, DownLeft2Msk, DownRight1Msk, DownRight2Msk;
extern Pixmap UpTogi1Xbm, UpTogi2Xbm, DownTogi1Xbm, DownTogi2Xbm, LeftTogi1Xbm;
extern Pixmap UpTogi1Msk, UpTogi2Msk, DownTogi1Msk, DownTogi2Msk, LeftTogi1Msk;
extern Pixmap LeftTogi2Xbm, RightTogi1Xbm, RightTogi2Xbm;
extern Pixmap LeftTogi2Msk, RightTogi1Msk, RightTogi2Msk;

extern GC Mati2GC;
extern GC Jare2GC, Kaki1GC, Kaki2GC, Mati3GC, Sleep1GC, Sleep2GC;
extern GC AwakeGC;
extern GC Up1GC, Up2GC, Down1GC, Down2GC, Left1GC, Left2GC, Right1GC, Right2GC;
extern GC UpLeft1GC, UpLeft2GC, UpRight1GC, UpRight2GC, DownLeft1GC, DownLeft2GC;
extern GC DownRight1GC, DownRight2GC;
extern GC UpTogi1GC, UpTogi2GC, DownTogi1GC, DownTogi2GC, LeftTogi1GC;
extern GC LeftTogi2GC, RightTogi1GC, RightTogi2GC;

extern unsigned int WindowWidth;
extern unsigned int WindowHeight;

extern XColor theForegroundColor;
extern XColor theBackgroundColor;

extern int Synchronous;
extern volatile sig_atomic_t TerminationRequested;

extern char *Foreground;
extern char *Background;
extern long IntervalTime;
extern double NekoSpeed;
extern int NoShape;
extern int ReverseVideo;

extern Bool DontMapped;
extern int NekoTickCount;
extern int NekoStateCount;
extern int NekoState;

extern XRRMonitorInfo *Monitors;
extern int MonitorCount;

extern int RestrictMonitor;
extern int TargetX;
extern int TargetY;
extern int ForceTargetFlag;
extern int ForceTargetX;
extern int ForceTargetY;

extern int BedX;
extern int BedY;
extern int GoingToBed;
extern Bool UseBed;

extern int DraggingBed;
extern int DragOffsetX;
extern int DragOffsetY;
extern int DebugMode;

extern int Waiting;
extern time_t NextMoveTime;

extern int Zoomies;
extern time_t ZoomiesEndTime;

extern int MinWait;
extern int MaxWait;

extern int NekoX;
extern int NekoY;
extern int NekoMoveDx;
extern int NekoMoveDy;
extern int NekoLastX;
extern int NekoLastY;
extern GC NekoLastGC;

extern int RaiseWindowDelay;
extern double SinPiPer8Times3;
extern double SinPiPer8;

void InitBitmapAndGCs(void);
void GetResources(void);
void SetupColors(void);
void InitMonitors(void);
void PlaceBedOnMonitor(void);
void InitScreen(char *DisplayName);
void RestoreCursor(void);
void RestoreCursorHandler(int signum);
void Interval(void);
Bool LoadBedPosition(void);
void SaveBedPosition(void);
void TickCount(void);
void SetNekoState(int SetValue);
void DrawNeko(int x, int y, Animation DrawAnime);
void RedrawNeko(void);
void NekoDirection(void);
Bool IsWindowOver(void);
Bool IsNekoDontMove(void);
Bool IsNekoMoveStart(void);
void PickRandomTarget(void);
int RectOnMonitor(int x, int y, int w, int h);
void PickNearbyTarget(int radius);
void CalcDxDy(void);
void NekoThinkDraw(void);
Bool ProcessKeyPress(XKeyEvent *theKeyEvent);
Bool ProcessEvent(void);
void ProcessNeko(void);
void Usage(void);
void GetArguments(int argc, char *argv[], char *theDisplayName);
int ParseLongOption(const char *option, const char *value, long minimum, long *result);
int ParseDoubleOption(const char *option, const char *value, double minimum, double *result);

#endif /* ONEKO_H */
