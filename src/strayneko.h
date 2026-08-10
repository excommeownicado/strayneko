#ifndef STRAYNEKO_H
#define STRAYNEKO_H

#include "core_state.h"
#include "geometry.h"
#include "monitor.h"
#include "config.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xrandr.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

#define AVAIL_KEYBUF 255
#define DEFAULT_FOREGROUND "black"
#define DEFAULT_BACKGROUND "white"
#define MAXDISPLAYNAME (64 + 5)
#define NOTDEFINED (-1)
#define IsTrue(str) (strcmp((str), "true") == 0)

typedef struct { GC gc; Pixmap pixmap; Pixmap mask; } Sprite;
typedef struct { const unsigned char *bits; const unsigned char *mask_bits; } SpriteData;

typedef struct {
    int x, y;
    int going_to_bed;
    Bool enabled;
    int dragging;
    int drag_offset_x, drag_offset_y;
    GC gc;
    Pixmap pixmap;
    Pixmap mask;
} BedData;

extern NekoData Neko;
extern BedData Bed;
extern ConfigData Config;
extern const char *ClassName;
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
void ClampBedPosition(void);
void PlaceBedOnMonitor(void);
void InitScreen(char *DisplayName);
void Cleanup(void);
void CleanupHandler(int signum);
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
void PickNearbyTarget(int radius);
void CalcDxDy(void);
void NekoThinkDraw(void);
Bool ProcessKeyPress(XKeyEvent *theKeyEvent);
void ProcessNeko(void);
void Usage(void);

#endif
