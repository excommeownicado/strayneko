#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "parser.h"

int
main(void)
{
    long value;
    double number;

    assert(ParseLongOption("123", 0, &value));
    assert(value == 123);

    assert(ParseLongOption("0", 0, &value));
    assert(value == 0);

    assert(!ParseLongOption("-1", 0, &value));
    assert(!ParseLongOption("abc", 0, &value));
    assert(!ParseLongOption("", 0, &value));

    assert(ParseDoubleOption("1.5", 0.0, &number));
    assert(number == 1.5);

    assert(!ParseDoubleOption("-1.5", 0.0, &number));
    assert(!ParseDoubleOption("abc", 0.0, &number));

    printf("parser tests passed\n");

    return 0;
}