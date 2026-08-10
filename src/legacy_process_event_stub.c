/*
 * Temporary compatibility symbol for the now-unused legacy ProcessNeko()
 * that remains in strayneko.c during the migration to PlatformEvent.
 *
 * The legacy ProcessNeko() is renamed by CMake and is never called by the
 * executable.  Keep this symbol only until that dead function is removed.
 */
int
ProcessEvent(void)
{
    return 0;
}
