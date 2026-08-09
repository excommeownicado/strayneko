#ifndef STRAYNEKO_PLATFORM_H
#define STRAYNEKO_PLATFORM_H

#include <stdint.h>

/*
 * Platform-neutral timing primitives.
 *
 * The implementation is intentionally kept outside the core so that the
 * event loop can later use a Windows, Wayland, or other platform backend
 * without changing gameplay code.
 */
uint64_t PlatformGetTimeMs(void);
void PlatformSleepUs(uint64_t microseconds);

#endif /* STRAYNEKO_PLATFORM_H */
