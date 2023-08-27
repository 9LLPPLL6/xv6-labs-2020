#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2,"Usage: sleep need a argument");
        exit(1);
    }
    int argument = atoi (argv[1]);
    if(argument<0)
    {
        fprintf(2,"Usage: sleep argument must > 0");
        exit(1);
    }
    sleep(argument);
    exit(0);
}