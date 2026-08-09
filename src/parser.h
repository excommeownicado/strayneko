#ifndef STRAYNEKO_PARSER_H
#define STRAYNEKO_PARSER_H

#include <stdbool.h>

int ParseLongOption(
    const char *value,
    long minimum,
    long *result
);

int ParseDoubleOption(
    const char *value,
    double minimum,
    double *result
);

bool ParseBoolOption(
    const char *value,
    bool *result
);

#endif /* STRAYNEKO_PARSER_H */
