#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

void readline(char **cmdline, int index)
{
    char buf[1024];
    int p = 0;
    while(read(0, buf+p, 1)){
        if(buf[p] != '\n'){
            p++;
        }
        else{
            buf[p] = 0;
            cmdline[index] = buf;
            cmdline[++index] = 0;
            break;
        }
    }
}



int main(int argc, char *argv[])
{
    if (argc < 3){
        fprintf(2, "Usage: \n");
    }

    char *new_arg[MAXARG];
    char *command = malloc(sizeof(argv[1])+1);
    strcpy(command, argv[1]);

    int j = 0;
    for(int i=1 ; i<argc ; i++){
        new_arg[j++] = argv[i];
    }

    readline(new_arg, argc-1);

    if (fork() == 0)
    {
        exec(command, new_arg);
        exit(1);
    }
    wait(0);
    return 0;
}



