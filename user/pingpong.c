#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


char* intToString(int num,char *str)
{
  int i = 0;
  do{
    str[i++] = num%10+48;
    num = num/10;
  }while(num);
  str[i] = '\0';
  int j = 0;
  char c = 'c';
  for(;j<i/2;j++){
    c = str[j];
    str[j] = str[i-j-1];
    str[i-j-1] = c; 
  }
  return str;
}

int
main(int argc, char *argv[])
{
    int pipedL[2];
    int pipedR[2];
    if(pipe(pipedL) != 0)
    {
        fprintf(2,"pipe error!!!");
        exit(1);
    }
    if(pipe(pipedR) != 0)
    {
        fprintf(2,"pipe error!!!");
        exit(1);
    }
    int parentPid = getpid();
    int cpid = fork();

    if(cpid == 0){
        char c = 'c';
        char* pid = "";
        intToString(parentPid,pid);
        close(pipedL[1]);
        close(pipedR[0]);
        if(read(pipedL[0],&c,1)){
            fprintf(2,"%s: received ping\n",pid);
            write(pipedR[1],&c, 1);
        }
        exit(0);
    }else{
        char c = 'c';
        char* pid = "";
        intToString(cpid,pid);
        close(pipedL[0]);
        close(pipedR[1]);
        write(pipedL[1],&c,1);
        if(read(pipedR[0],&c,1)){
            fprintf(2,"%s: received pong\n",pid);
        }
        exit(0);
    }
}