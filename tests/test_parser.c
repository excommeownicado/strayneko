#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "parser test failed: %s\n", #condition); \
            return EXIT_FAILURE; \
        } \
    } while (0)

int
main(void)
{
    long value;
    double number;
    bool boolean;

    CHECK(ParseLongOption("123", 0, &value));
    CHECK(value == 123);

    CHECK(ParseLongOption("0", 0, &value));
    CHECK(value == 0);

    CHECK(!ParseLongOption("-1", 0, &value));
    CHECK(!ParseLongOption("abc", 0, &value));
    CHECK(!ParseLongOption("", 0, &value));

    CHECK(ParseDoubleOption("1.5", 0.0, &number));
    CHECK(number == 1.5);

    CHECK(!ParseDoubleOption("-1.5", 0.0, &number));
    CHECK(!ParseDoubleOption("abc", 0.0, &number));

    CHECK(ParseBoolOption("true", &boolean));
    CHECK(boolean);

    CHECK(ParseBoolOption("false", &boolean));
    CHECK(!boolean);

    CHECK(!ParseBoolOption("True", &boolean));
    CHECK(!ParseBoolOption("1", &boolean));

    puts("parser tests passed");
    return EXIT_SUCCESS;
}
