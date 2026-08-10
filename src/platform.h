#ifndef STRAYNEKO_PLATFORM_H
#define STRAYNEKO_PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

#include "platform_event.h"

/*
 * Platform-neutral primitives.
 *
 * The implementation is kept outside the core so the application loop can
 * use a different timing and event backend without knowing platform details.
 */
uint64_t PlatformGetTimeMs(void);
void PlatformSleepUs(uint64_t microseconds);
bool PlatformPollEvent(PlatformEvent *event);

#endif /* STRAYNEKO_PLATFORM_H */
