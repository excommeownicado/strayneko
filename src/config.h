#ifndef STRAYNEKO_CONFIG_H
#define STRAYNEKO_CONFIG_H

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

#endif
