#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int ticks = 0;
  if(argc != 2){
    fprintf(2, "sleep parameter error...\n");
    exit(1);
  }
  ticks = atoi(argv[1]);
  sleep(ticks);
  exit(0);
}