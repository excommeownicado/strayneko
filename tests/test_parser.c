#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "parser.h"

int
main(void)
{
    long value;
    double number;

    assert(ParseLongOption("--test", "123", 0, &value));
    assert(value == 123);

    assert(ParseLongOption("--test", "0", 0, &value));
    assert(value == 0);

    assert(!ParseLongOption("--test", "-1", 0, &value));
    assert(!ParseLongOption("--test", "abc", 0, &value));
    assert(!ParseLongOption("--test", "", 0, &value));

    assert(ParseDoubleOption("--test", "1.5", 0.0, &number));
    assert(number == 1.5);

    assert(!ParseDoubleOption("--test", "-1.5", 0.0, &number));
    assert(!ParseDoubleOption("--test", "abc", 0.0, &number));

    printf("parser tests passed\n");

    return 0;
}