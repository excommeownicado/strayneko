#ifndef STRAYNEKO_CONFIG_H
#define STRAYNEKO_CONFIG_H

#include <stdbool.h>

typedef struct {
    char *foreground;
    char *background;

    long interval_time;
    bool interval_time_set;

    double speed;
    bool speed_set;

    int no_shape;

    int min_wait;
    int max_wait;

    int restrict_monitor;
} ConfigData;

#endif
