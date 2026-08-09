#include <assert.h>
#include <stdio.h>

#include "geometry.h"

static void
TestPointInRect(void)
{
    Rect rect = {10, 20, 100, 50};

    assert(PointInRect((Point){10, 20}, rect));
    assert(PointInRect((Point){109, 69}, rect));
    assert(!PointInRect((Point){110, 69}, rect));
    assert(!PointInRect((Point){109, 70}, rect));
}

static void
TestRectIntersectsRect(void)
{
    Rect a = {0, 0, 32, 32};

    assert(RectIntersectsRect(a, (Rect){16, 16, 32, 32}));
    assert(RectIntersectsRect(a, (Rect){31, 0, 32, 32}));
    assert(!RectIntersectsRect(a, (Rect){32, 0, 32, 32}));
    assert(!RectIntersectsRect(a, (Rect){0, 32, 32, 32}));
}

static void
TestRectInsideRect(void)
{
    Rect container = {0, 0, 100, 100};

    assert(RectInsideRect((Rect){0, 0, 32, 32}, container));
    assert(RectInsideRect((Rect){68, 68, 32, 32}, container));
    assert(!RectInsideRect((Rect){69, 68, 32, 32}, container));
    assert(!RectInsideRect((Rect){-1, 0, 32, 32}, container));
}

static void
TestClampRectToRect(void)
{
    Rect container = {100, 200, 300, 200};

    Rect result = ClampRectToRect(
        (Rect){50, 150, 32, 32},
        container
    );

    assert(result.x == 100);
    assert(result.y == 200);

    result = ClampRectToRect(
        (Rect){390, 390, 32, 32},
        container
    );

    assert(result.x == 368);
    assert(result.y == 368);
}

int
main(void)
{
    TestPointInRect();
    TestRectIntersectsRect();
    TestRectInsideRect();
    TestClampRectToRect();

    puts("geometry tests passed");
    return 0;
}
