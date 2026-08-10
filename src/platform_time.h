#ifndef STRAYNEKO_PLATFORM_TIME_H
#define STRAYNEKO_PLATFORM_TIME_H

#include <stdint.h>

uint64_t PlatformGetTimeMs(void);
void PlatformSleepUs(uint64_t microseconds);

#endif
