#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

void prime(int p[2])
{
    int primes;
    int num;
    close(p[1]);
    if(read(p[0], &primes, 4)!=4){
        fprintf(2,"Unable to read number from neighbor process\n");
        exit(1);
    }
    printf("prime: %d\n", primes);

    if(read(p[0], &num, 4)){
        int cp[2];
        pipe(cp);
        if(fork() == 0){
            prime(cp);
        }

        close(cp[0]);
        if(num % primes){
            write(cp[1], &num, 4);
        }
        while(read(p[0], &num, 4)){
            if(num % primes){
                write(cp[1], &num, 4);
            }
        }
        close(p[0]);
        close(cp[1]);
        wait(0);
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    if(fork()==0){
        prime(p);
    }
    close(p[0]);
    for(int i=2 ; i<=35 ; i++){
        if(write(p[1], &i, 4)!=4){
            fprintf(2, "Unable to write numbers into pipe\n");
        }
    }
    close(p[1]);
    wait(0);
    exit(0);

    return 0;
}