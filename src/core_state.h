#ifndef STRAYNEKO_CORE_STATE_H
#define STRAYNEKO_CORE_STATE_H

#include <time.h>
#include <X11/Xlib.h>

#define BITMAP_WIDTH 32
#define BITMAP_HEIGHT 32
#define MAX_TICK 9999
#define DEFAULT_RAISE_WAIT 16

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

typedef enum {
    SPRITE_MATI2, SPRITE_JARE2, SPRITE_KAKI1, SPRITE_KAKI2,
    SPRITE_MATI3, SPRITE_SLEEP1, SPRITE_SLEEP2, SPRITE_AWAKE,
    SPRITE_UP1, SPRITE_UP2, SPRITE_DOWN1, SPRITE_DOWN2,
    SPRITE_LEFT1, SPRITE_LEFT2, SPRITE_RIGHT1, SPRITE_RIGHT2,
    SPRITE_UPLEFT1, SPRITE_UPLEFT2, SPRITE_UPRIGHT1, SPRITE_UPRIGHT2,
    SPRITE_DOWNLEFT1, SPRITE_DOWNLEFT2, SPRITE_DOWNRIGHT1, SPRITE_DOWNRIGHT2,
    SPRITE_UPTOGI1, SPRITE_UPTOGI2, SPRITE_DOWNTOGI1, SPRITE_DOWNTOGI2,
    SPRITE_LEFTTOGI1, SPRITE_LEFTTOGI2, SPRITE_RIGHTTOGI1, SPRITE_RIGHTTOGI2,
    SPRITE_COUNT
} SpriteID;

typedef struct {
    int x, y, target_x, target_y;
    int move_dx, move_dy;
    int last_x, last_y;
    GC last_gc;
    SpriteID last_sprite;
    int tick_count, state_count, state;
    int waiting;
    time_t next_move_time;
    int zoomies;
    time_t zoomies_end_time;
    int raise_window_delay;
} NekoData;

#endif /* STRAYNEKO_CORE_STATE_H */
