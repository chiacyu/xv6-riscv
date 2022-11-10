#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"


int
main(int argc, char *argv[])
{
    int p[2];
    int pid;
    pipe(p);

    if(fork() == 0){
        char buf_c[2];
        char *msg_c = "c";
        pid = getpid();
        if (read(p[0], buf_c, 1)!=1)
        {
            fprintf(2, "Child read pipe error\n");
        }
        close(p[0]);
        printf("%d : receive ping\n",pid);
        if(write(p[1], msg_c, 1) !=1){
            fprintf(2, "Child write to pipe error\n");
        }
        close(p[1]);
        exit(1);
    }
    char buf_p[2];
    char *msg_p = "p";
    pid = getpid();
    if(write(p[1], msg_p, 1) != 1){
        fprintf(2, "Parent can't write msg to the pipe\n");
    }
    close(p[1]);
    if( read(p[0], buf_p, 1) != 1){
        fprintf(2, "Can't read char from pipe\n");
    }
    close(p[0]);
    printf("%d : receive pong\n",pid);
    exit(0);
}