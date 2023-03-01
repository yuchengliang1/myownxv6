#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int getLeftParam(int fdRead){
    int param = 0;
    int readLen = read(fdRead,&param,sizeof(param));
    if(readLen == 0 || readLen == -1){
        return -1;
    } else {
        return param;
    }
}

void transParam(int param,int fdWrite){
    write(fdWrite,&param,sizeof(param));
}

void processLoop(int fdRead)
{
    int firstParam = getLeftParam(fdRead);
    if(firstParam == -1){
        close(fdRead);
        return;
    }
    int fdRw[2];
    pipe(fdRw);
    fprintf(2,"prime %d\n",firstParam);
    int pid = fork();
    if(pid != 0){
        close(fdRw[0]);
        int cparam = getLeftParam(fdRead);
        int pidd = 0;
        while(cparam != -1){
            if(cparam % firstParam != 0){
                transParam(cparam,fdRw[1]);
            }
            cparam = getLeftParam(fdRead);
        }
        close(fdRw[1]);
        wait(&pidd);
        exit(0);
    }else{
        close(fdRw[1]);
        processLoop(fdRw[0]);
        exit(0);
    }
}

int
main(int argc, char *argv[])
{
    int nums[34];
    int fd[2];
    pipe(fd);
    for(int i=0;i<=33;i++){
        nums[i] = i+2;
        write(fd[1],&nums[i],sizeof(nums[i]));
    }
    close(fd[1]);
    processLoop(fd[0]);
    exit(0);
}