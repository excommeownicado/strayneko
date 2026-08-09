#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

int
ParseLongOption(
    const char *value,
    long minimum,
    long *result)
{
    char *end;
    long parsed;

    errno = 0;
    parsed = strtol(value, &end, 10);

    if (end == value ||
        *end != '\0' ||
        errno == ERANGE ||
        parsed < minimum) {
        return 0;
    }

    *result = parsed;

    return 1;
}

int
ParseDoubleOption(
    const char *value,
    double minimum,
    double *result)
{
    char *end;
    double parsed;

    errno = 0;
    parsed = strtod(value, &end);

    if (end == value ||
        *end != '\0' ||
        errno == ERANGE ||
        !isfinite(parsed) ||
        parsed < minimum) {
        return 0;
    }

    *result = parsed;

    return 1;
}

bool
ParseBoolOption(const char *value, bool *result)
{
    if (value == NULL || result == NULL) {
        return false;
    }

    if (strcmp(value, "true") == 0) {
        *result = true;
        return true;
    }

    if (strcmp(value, "false") == 0) {
        *result = false;
        return true;
    }

    return false;
}
