#include <stdio.h>
#include <stdlib.h>

#include "geometry.h"

#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "geometry test failed: %s\n", #condition); \
            return 1; \
        } \
    } while (0)

static int
TestPointInRect(void)
{
    Rect rect = {10, 20, 100, 50};

    CHECK(PointInRect((Point){10, 20}, rect));
    CHECK(PointInRect((Point){109, 69}, rect));
    CHECK(!PointInRect((Point){110, 69}, rect));
    CHECK(!PointInRect((Point){109, 70}, rect));
    return 0;
}

static int
TestRectIntersectsRect(void)
{
    Rect a = {0, 0, 32, 32};

    CHECK(RectIntersectsRect(a, (Rect){16, 16, 32, 32}));
    CHECK(RectIntersectsRect(a, (Rect){31, 0, 32, 32}));
    CHECK(!RectIntersectsRect(a, (Rect){32, 0, 32, 32}));
    CHECK(!RectIntersectsRect(a, (Rect){0, 32, 32, 32}));
    return 0;
}

static int
TestRectInsideRect(void)
{
    Rect container = {0, 0, 100, 100};

    CHECK(RectInsideRect((Rect){0, 0, 32, 32}, container));
    CHECK(RectInsideRect((Rect){68, 68, 32, 32}, container));
    CHECK(!RectInsideRect((Rect){69, 68, 32, 32}, container));
    CHECK(!RectInsideRect((Rect){-1, 0, 32, 32}, container));
    return 0;
}

static int
TestClampRectToRect(void)
{
    Rect container = {100, 200, 300, 200};
    Rect result = ClampRectToRect(
        (Rect){50, 150, 32, 32},
        container
    );

    CHECK(result.x == 100);
    CHECK(result.y == 200);

    result = ClampRectToRect(
        (Rect){390, 390, 32, 32},
        container
    );

    CHECK(result.x == 368);
    CHECK(result.y == 368);

    result = ClampRectToRect(
        (Rect){-2000, -1200, 32, 32},
        (Rect){-1920, -1080, 1920, 1080}
    );

    CHECK(result.x == -1920);
    CHECK(result.y == -1080);

    result = ClampRectToRect(
        (Rect){500, 500, 500, 500},
        (Rect){0, 0, 100, 100}
    );

    CHECK(result.x == 0);
    CHECK(result.y == 0);
    return 0;
}

int
main(void)
{
    if (TestPointInRect() != 0 ||
        TestRectIntersectsRect() != 0 ||
        TestRectInsideRect() != 0 ||
        TestClampRectToRect() != 0) {
        return EXIT_FAILURE;
    }

    puts("geometry tests passed");
    return EXIT_SUCCESS;
}
