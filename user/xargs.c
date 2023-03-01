#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

void execParam(int argc, char *argv[],char* p){
    char *argss[52] = {0};
    int i =0;
    for(;i<argc-1;i++){
        argss[i] = argv[i+1];
    }
    argss[i] = p;
    if(fork() == 0){
        // fprintf(1,"argss 0: %s\n",argss[0]);
        // fprintf(1,"argss 1: %s\n",argss[1]);
        // fprintf(1,"argss 2: %s\n",argss[2]);
        // fprintf(1,"argss 3: %s\n",argss[3]);
        // fprintf(1,"current: %s\n",p);
        //fprintf(1,"cmd: %s\n",argv[1]);
        exec(argv[1],argss);
        exit(0);
    }else{
        return;
    }
}

int
main(int argc, char *argv[])
{
    char cstr[MAXARG];
    char c;
    char *p = cstr;
    int count =0;
    while(read(0,&c,sizeof(char)) == sizeof(char)){
        if((c == '\n' || c == '\0' || c == 0 ) && count >= 2){
            *p = '\0';
            //fprintf(1,"pointer: %s",cstr);
            execParam(argc,argv,cstr);
            p = cstr;
            count = 0;
            if(c == '\0'){
                exit(0);
            }
        } else {
            *p = c;
            p++;
            count++;
        }
    }
    exit(0);
}