#define _POSIX_C_SOURCE 200809L
#include "custom_descriptor.h"

#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct custom_FILE *custom_fopen(const char *path)
{
    struct custom_FILE *f = malloc(sizeof(struct custom_FILE));
    if (!f) // The program should not have allocated anything at that point
        errx(2, "Could not allocate memory !");
    if (!path)
    {
        f->fd = CUSTOM_FD;
        f->index = 0;
        f->str = NULL;
        return f;
    }
    FILE *file = fopen(path, "r");
    if (!file)
    {
        free(f);
        errx(127, "Cannot open file: %s", path);
    }
    f->fd = file->_fileno;
    f->file = file;
    f->str = NULL;
    return f;
}

struct custom_FILE *createfrom_string(char *str)
{
    struct custom_FILE *f = custom_fopen(NULL);
    f->str = strdup(str);
    f->file = NULL;
    f->len = strlen(str);
    return f;
}

void custom_fclose(struct custom_FILE *f)
{
    if (f->fd == CUSTOM_FD)
        free(f->str);
    else
        fclose(f->file);
    free(f);
}

char *custom_fgets(char *s, size_t size, struct custom_FILE *f)
{
    if (f->fd != CUSTOM_FD)
        return fgets(s, size, f->file);
    if (f->index >= f->len)
        return NULL;
    for (size_t i = 0; i < size && f->index < f->len; i++)
    {
        s[i] = f->str[f->index];
        f->index++;
        if (f->str[f->index - 1] == '\n' || f->str[f->index - 1] == 0)
            return s;
    }
    return s;
}

char *custom_getfile(struct custom_FILE *f)
{
    if (f->fd == CUSTOM_FD)
        return strdup(f->str);
    int size = fseek(f->file, 0, SEEK_END);
    size = ftell(f->file);
    fseek(f->file, 0, SEEK_SET);

    char *getfile = malloc(size);

    if (fread(getfile, sizeof(char), size, f->file) == 0)
        errx(2, "Custom getfile: could not open file\n");

    return getfile;
}