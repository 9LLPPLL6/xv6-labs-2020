#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

char *args[MAXARG];

int main(int argc, char *argv[])
{
    char line[512];
    char ch, *p;
    int eof = 1;
    int num, i, pid, sate;

    for (i = 1; i <= argc; i++)
        args[i - 1] = argv[i];

    while (eof)
    {
        num = argc - 1;

        for (i = 0; ((eof = read(0, &ch, 1)) != 0) && (ch != '\n'); i++)
        {
            if (i + 1 >= 512)
            {
                fprintf(2, "xargs: line too long\n");
                exit(1);
            }
            line[i] = ch;
        }

        if ((eof == 0) && (i == 0))
            break;

        line[i] = 0;
        p = line;

        for (i = 0; line[i] != 0; i++)
            if (line[i] == ' ')
            {
                line[i] = 0;
                args[num] = malloc(strlen(p) + 1);
                strcpy(args[num], p);
                p = p + strlen(p) + 1;
                num++;
                if (num + 2 >= MAXARG)
                {
                    fprintf(2, "xargs: the num of arg large MAX\n");
                    exit(1);
                }
            }

        args[num] = malloc(strlen(p) + 1);
        strcpy(args[num], p);
        args[++num] = 0;

        if ((pid = fork()) == 0)
            exec(args[0], args);

        wait(&sate);

        for (i = argc; i < num; i++)
            free(args[i]);
    }
    exit(0);
}