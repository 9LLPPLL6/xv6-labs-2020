#include "kernel/types.h"
#include "user/user.h"

void prime(int f)
{
    char buf[136];
    char *p = buf;
    int *num, a, i, *pi, statu;
    int pf[2];
    while (read(f, p, 4) != 0)
        p += 4;
    close(f);
    *((int *)p) = 0;
    num = (int *)buf;
    a = num[0];
    printf("prime %d\n", a);
    i = 0;
    pi = num + 1;
    while (*pi != 0)
    {
        if ((*pi % a) != 0)
            num[i++] = *pi;
        pi++;
    }

    if (i == 0)
        return;

    num[i] = 0;
    pipe(pf);
    write(pf[1], buf, 136);
    close(pf[1]);

    if (fork() == 0)
        prime(p[0]);
    
    wait(&statu);
}

int main()
{
    int num[34];
    int *pn, i, statu;
    int p[2];
    printf("prime 2\n");
    pn = num;
    for (i = 3; i <= 35; i++)
        if (i % 2 != 0)
        {
            *pn = i;
            pn++;
        }
    *pn = 0;
    pipe(p);
    write(p[1], (char *)num, 136);
    close(p[1]);
    close(0);
    if (fork() == 0)
    {
        prime(p[0]);
        exit(0);
    }
    wait(&statu);
    exit(0);
}