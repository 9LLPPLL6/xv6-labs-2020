#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

static void find(char *path, char *name)
{
    int fd, fd1;
    char fpname[512];
    char *p, *p1;
    struct stat st, st1;
    struct dirent de;

    if ((fd = open(path, 0)) == -1)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    
    if (fstat(fd, &st) == -1)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    strcpy(fpname, path);
    p = fpname + strlen(path);

    switch (st.type)
    {
    case T_FILE:
        p1 = p;
        for (; p1 >= fpname && *p1 != '/'; p1--)
            ;
        p1++;
        if (strcmp(name, p1) == 0)
            fprintf(1, "%s\n", path);
        break;

    case T_DIR:
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            *p++ = '/';
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            if (strcmp(name, p) == 0)
                fprintf(1, "%s\n", fpname);

            if (!strcmp(p, ".") || !strcmp(p, ".."))
                ;
            else if ((fd1 = open(fpname, 0)) == -1)
                fprintf(2, "find: cannot open %s\n", fpname);
            else if (fstat(fd1, &st1) == -1)
            {
                fprintf(2, "find: cannot stat %s\n", fpname);
                close(fd1);
            }
            else if (st1.type == T_DIR)
            {
                find(fpname, name);
                close(fd1);
            }
            else
                close(fd1);
            p = fpname + strlen(path);
            *p = 0;
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        exit(0);
    char *name = argv[argc - 1];
    if (argc == 2)
        find(".", name);
    else
        for (int i = 1; i < argc - 1; i++)
            find(argv[i], name);

    exit(0);
}