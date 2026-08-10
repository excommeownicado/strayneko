#include "strayneko.h"

#include <sys/stat.h>

static char *
GetBedConfigPath(void)
{
    char *config_home;
    char *home;
    static char path[PATH_MAX];

    config_home = getenv("XDG_CONFIG_HOME");
    if (config_home != NULL && config_home[0] != '\0') {
        snprintf(path, sizeof(path), "%s/strayneko/bedpos", config_home);
        return path;
    }

    home = getenv("HOME");
    if (home != NULL && home[0] != '\0') {
        snprintf(path, sizeof(path), "%s/.config/strayneko/bedpos", home);
        return path;
    }

    return NULL;
}

static int
MakeDirectoryPath(const char *path)
{
    char dir[PATH_MAX];
    char *slash;
    struct stat st;

    if (path == NULL || path[0] == '\0') {
        return 0;
    }

    strncpy(dir, path, sizeof(dir));
    dir[sizeof(dir) - 1] = '\0';
    slash = strrchr(dir, '/');
    if (slash == NULL) {
        return 0;
    }
    *slash = '\0';

    for (char *p = dir + 1; *p != '\0'; p++) {
        if (*p != '/') {
            continue;
        }

        *p = '\0';

        if (stat(dir, &st) != 0) {
            if (mkdir(dir, 0700) != 0 && errno != EEXIST) {
                return 0;
            }
        } else if (!S_ISDIR(st.st_mode)) {
            return 0;
        }

        *p = '/';
    }

    if (stat(dir, &st) != 0) {
        if (mkdir(dir, 0700) != 0 && errno != EEXIST) {
            return 0;
        }
    } else if (!S_ISDIR(st.st_mode)) {
        return 0;
    }

    return 1;
}

Bool
LoadBedPosition(void)
{
    char *path;
    FILE *file;
    int x;
    int y;

    path = GetBedConfigPath();
    if (path == NULL) {
        return False;
    }

    file = fopen(path, "r");
    if (file == NULL) {
        return False;
    }

    if (fscanf(file, "%d %d", &x, &y) != 2) {
        fclose(file);
        return False;
    }

    fclose(file);

    Bed.x = x;
    Bed.y = y;
    return True;
}

void
SaveBedPosition(void)
{
    char *path;
    FILE *file;

    ClampBedPosition();

    path = GetBedConfigPath();
    if (path == NULL || !MakeDirectoryPath(path)) {
        return;
    }

    file = fopen(path, "w");
    if (file == NULL) {
        return;
    }

    if (fprintf(file, "%d %d\n", Bed.x, Bed.y) < 0) {
        fclose(file);
        return;
    }

    fclose(file);
}
