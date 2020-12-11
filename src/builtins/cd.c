#define _POSIX_C_SOURCE 200809L

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *update_path(char *path, char *section)
{
    printf("path = %s, section = %s\n", path, section);
    char *new = NULL;
    if (!strcmp(section, ".."))
    {
        char *last_slash = strrchr(path, '/');
        *last_slash = '\0';
        new = strdup(path);
    }
    else if (!strcmp(section, "~"))
    {
        new = strdup(getenv("HOME"));
    }
    else if (!strcmp(section, "/"))
    {
        new = strdup("/");
    }
    else if (!strcmp(section, "-"))
    {
        new = strdup(getenv("OLDPWD"));
    }
    else
    {
        new = strdup(path);
        new = realloc(new, sizeof(char) * (strlen(new) + strlen(section) + 2));
        if (new[strlen(new) - 1] != '/')
            strcat(new, "/");
        strcat(new, section);
    }
    printf("[NEW] = %s\n", new);
    free(path);
    return new;
}

static int argv_len(char *argv[])
{
    int i = 0;
    while (argv && argv[i])
        i++;
    return i;
}

static char *get_path_destination(int argc, char **argv)
{
    char *path = strdup(getenv("PWD"));

    if (!argc)
        path = update_path(path, "~");

    else if (argv[0][0] == '/')
        path = update_path(path, "/");

    char *path_to_iter = argv[0];
    char *next_section = NULL;

    while ((next_section = strtok(path_to_iter, "/")))
    {
        path = update_path(path, next_section);
        path_to_iter = NULL;
    }
    return path;
}

char *b_cd(char **argv)
{
    int i = argv_len(argv);

    if (i > 2)
    {
        printf("%d\n", i);
        errx(1, "cd: too many arguments");
    }

    return get_path_destination(i - 1, argv + 1);
}