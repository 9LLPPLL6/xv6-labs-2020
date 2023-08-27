#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char data[1];
    int p1[2];
    int p2[2];
    int ppid, cpid;
    if (pipe(p1) < 0)
    {
        fprintf(2, "Usage: pingpong pipe fail\n");
        exit(1);
    }
    if (pipe(p2) < 0)
    {
        fprintf(2, "Usage: pingpong pipe fail\n");
        exit(1);
    }
    write(p1[1], "a", 1);
    close(p1[1]);
    if ((cpid = fork()) == 0)
    {
        read(p1[0], data, 1);
        cpid = getpid();
        fprintf(1, "%d: received ping\n", cpid);
        write(p2[1], data, 1);
        close(p2[1]);
        exit(0);
    }
    ppid = getpid();
    read(p2[0], data, 1);
    fprintf(1, "%d: received pong\n", ppid);
    exit(0);
}