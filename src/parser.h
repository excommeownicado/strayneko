#ifndef STRAYNEKO_PARSER_H
#define STRAYNEKO_PARSER_H

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

#endif /* STRAYNEKO_PARSER_H */