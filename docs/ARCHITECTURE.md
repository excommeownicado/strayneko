# strayneko architecture

## Layers

The codebase is intentionally split into three broad layers:

```text
Application / neko logic
        |
        v
Platform-neutral helpers
(geometry, parser, timing API)
        |
        v
Platform backend
(X11 event/window implementation)
```

### Application layer

The neko state machine, configuration, resource handling, bed behavior, and
main process loop live here. Application code should avoid adding new direct
X11 calls unless the operation is genuinely platform-specific.

### Platform-neutral helpers

`geometry.c` contains pure rectangle and point operations. It has no X11
dependencies and is therefore suitable for unit tests and reuse by future
backends.

`parser.c` contains command-line parsing helpers without platform event-loop
logic.

`platform.h` defines the timing interface used by the core. The current
implementation is POSIX (`platform_posix.c`). A future backend can provide the
same interface without changing the process loop.

### X11 backend

Files prefixed with `x11_` own X11 event translation and window operations.
`x11_platform_events.c` converts native X11 events into `PlatformEvent` values
consumed by the process loop.

## State ownership

The current branch is in a transitional state: legacy modules still expose
some application state through the existing `strayneko.h` interface. New
code should prefer the smallest existing module interface rather than adding
new globals. Further state encapsulation can then be done incrementally without
mixing it with the platform split.

## Testing

Pure geometry and parser behavior are covered by unit tests. CI also builds
Debug and Release configurations, runs the unit tests, builds with Clang, and
builds with AddressSanitizer and UndefinedBehaviorSanitizer enabled.
