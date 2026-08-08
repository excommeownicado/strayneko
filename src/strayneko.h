#ifndef STRAYNEKO_H
#define STRAYNEKO_H

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

#define NEKO_STATE_COUNT 18

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

typedef enum {
    SPRITE_MATI2,
    SPRITE_JARE2,
    SPRITE_KAKI1,
    SPRITE_KAKI2,
    SPRITE_MATI3,
    SPRITE_SLEEP1,
    SPRITE_SLEEP2,
    SPRITE_AWAKE,

    SPRITE_UP1,
    SPRITE_UP2,
    SPRITE_DOWN1,
    SPRITE_DOWN2,
    SPRITE_LEFT1,
    SPRITE_LEFT2,
    SPRITE_RIGHT1,
    SPRITE_RIGHT2,

    SPRITE_UPLEFT1,
    SPRITE_UPLEFT2,
    SPRITE_UPRIGHT1,
    SPRITE_UPRIGHT2,
    SPRITE_DOWNLEFT1,
    SPRITE_DOWNLEFT2,
    SPRITE_DOWNRIGHT1,
    SPRITE_DOWNRIGHT2,

    SPRITE_UPTOGI1,
    SPRITE_UPTOGI2,
    SPRITE_DOWNTOGI1,
    SPRITE_DOWNTOGI2,
    SPRITE_LEFTTOGI1,
    SPRITE_LEFTTOGI2,
    SPRITE_RIGHTTOGI1,
    SPRITE_RIGHTTOGI2,

    SPRITE_COUNT
} SpriteID;

typedef struct {
    GC gc;
    Pixmap pixmap;
    Pixmap mask;
} Sprite;

typedef struct {
    const unsigned char *bits;
    const unsigned char *mask_bits;
} SpriteData;

typedef struct {
    int x;
    int y;
    int target_x;
    int target_y;

    int move_dx;
    int move_dy;

    int last_x;
    int last_y;
    GC last_gc;
    SpriteID last_sprite;

    int tick_count;
    int state_count;
    int state;

    int waiting;
    time_t next_move_time;

    int zoomies;
    time_t zoomies_end_time;

    int raise_window_delay;
} NekoData;

typedef struct {
    int x;
    int y;

    int going_to_bed;
    Bool enabled;

    int dragging;
    int drag_offset_x;
    int drag_offset_y;

    GC gc;
    Pixmap pixmap;
    Pixmap mask;
} BedData;

typedef struct {
    char *foreground;
    char *background;

    long interval_time;
    double speed;

    int no_shape;

    int min_wait;
    int max_wait;

    int restrict_monitor;
} ConfigData;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} MonitorRect;

typedef struct {
    int min_x;
    int min_y;
    int max_x;
    int max_y;
} MonitorBounds;

extern NekoData Neko;
extern BedData Bed;
extern ConfigData Config;

extern char *ClassName;
extern char *ProgramName;

extern Display *theDisplay;
extern int theScreen;
extern unsigned int theDepth;
extern Window theRoot;
extern Window theWindow;
extern Window BedWindow;

extern unsigned int WindowWidth;
extern unsigned int WindowHeight;

extern Sprite Sprites[SPRITE_COUNT];

extern XColor theForegroundColor;
extern XColor theBackgroundColor;

extern int Synchronous;
extern volatile sig_atomic_t TerminationRequested;

extern Bool DontMapped;

extern XRRMonitorInfo *Monitors;
extern int MonitorCount;

extern int ForceTargetFlag;
extern int ForceTargetX;
extern int ForceTargetY;

#ifdef ENABLE_DEBUG
extern int DebugMode;
#else
#define DebugMode 0
#endif

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
void DrawNeko(int x, int y, int tick);
void RedrawNeko(void);
void NekoDirection(void);
Bool IsWindowOver(void);
Bool IsNekoDontMove(void);
Bool IsNekoMoveStart(void);
void PickRandomTarget(void);
MonitorRect GetMonitorRect(int monitor);
MonitorBounds GetMonitorBounds(int monitor);
int RectOnMonitor(int x, int y, int w, int h);
int FindMonitorFor(int x, int y);
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

#endif /* STRAYNEKO_H */
