#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <errno.h>
#include "common.h"
#include "list.h"

void help()
{
    printf("USAGE:\n");
    printf("     %s [OPTIONS] [DIR] ...\n\n", PROGRAM);
    printf("OPTIONS:\n");
    printf("    -a  show hidden files.\n");
    printf("    -d  show only directories.\n");
    printf("    -f  show only files.\n");
    printf("    -g  show groups of files.\n");
    printf("    -h  print help.\n");
    printf("    -l  show all information about the file (permissions, size,...).\n");
    printf("    -m  show modification time of each file.\n");
    printf("    -p  show permissions of files.\n");
    printf("    -r  used with -s, makes the size readable as 4K, 13M, 2G, etc.\n");
    printf("    -s  show sizes of files.\n");
    printf("    -t  show the contents of the subdirectories.\n");
    printf("        use the option \"t\" with DEPTH:\n");
    printf("            %s -[...]t [DEPTH] [DIR] ...\n", PROGRAM);
    printf("        or without:\n");
    printf("            %s -[...]t[...]  [DIR] ...\n", PROGRAM);
    printf("    -u  show the owners of files.\n");
    printf("    -v  version of lf command.\n\n");
}

void version()
{
    printf("Version: %s %s.\n", PROGRAM, VERSION);
}

void *lf_alloc(long int size)
{
    void *b;

    b = malloc(size);
    if (!b)
    {
        printf("%s: %s\n", PROGRAM, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return b;
}

// places the contents of the symbolic link pathname in the buffer "buf" (global variable).
// if failed, also print error in the buffer "buf"
bool lf_link(const char *nm)
{
    int cnt;
    cnt = readlink(path, buf, PATH_MAX - 1);
    if (cnt == -1)
    {
        strcpy(buf, strerror(errno));
        return false;
    }
    return true;
}

// if failed, also print error in the buffer "buf"
bool lf_stat(const char *nm, struct stat *s)
{
    if (lstat(nm, s) == -1)
    {
        strcpy(buf, strerror(errno));
        return false;
    }
    return true;
}

void lf_init()
{
    setbuf(stdout, NULL);
    // init PATH
    path = (char *)lf_alloc(sizeof(char) * PATH_MAX);
    // init BUF
    buf = (char *)lf_alloc(sizeof(char) * PATH_MAX);
    // init OPT
    memset(&opt, 0, OPTIONSIZ);
}

void lf_quit()
{
    free(path);
    free(buf);
    exit(EXIT_SUCCESS);
}

int has_space(const char *name)
{
    int i, tmp;

    tmp = strlen(name);
    for (i = 0; (i < tmp) && (name[i] != ' '); i++)
    {
    }
    return i != tmp;
}

int is_absolute_path(const char *pth)
{
    if (pth)
    {
        return (path[0] == '/');
    }
    return 0;
}

char *lfext(const char *tmp)
{
    if (!tmp)
    {
        return NULL;
    }
    int n = strlen(tmp) - 1, i = n, ok = 1;
    while (ok &&
           (i > 0) &&
           (tmp[i] != '.'))
    {
        if (tmp[i] == '/')
        {
            ok = 0;
        }
        else
        {
            --i;
        }
    }
    if (ok && i != n)
    {
        return (char *)&tmp[i + 1];
    }
    return NULL;
}