#include "platform_time.h"

#include <errno.h>
#include <time.h>

uint64_t
PlatformGetTimeMs(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0;
    }

    return (uint64_t)ts.tv_sec * 1000ULL
         + (uint64_t)ts.tv_nsec / 1000000ULL;
}

void
PlatformSleepUs(uint64_t microseconds)
{
    struct timespec request;

    request.tv_sec = (time_t)(microseconds / 1000000ULL);
    request.tv_nsec = (long)((microseconds % 1000000ULL) * 1000ULL);

    while (nanosleep(&request, &request) != 0 && errno == EINTR) {
        /* Resume the remaining sleep after an interrupted system call. */
    }
}
