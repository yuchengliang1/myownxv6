#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void find_dir(char *path,char *fileName)
{
  char buf[512], *p;
  struct dirent de;
  struct stat st;
  int fd;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "cannot stat %s\n", path);
    close(fd);
    return;
  }

  //fprintf(2,"current path: %s\n",path);

  switch(st.type){
  case T_FILE:
    if(strcmp(fileName,fmtname(path)) == 0){
       printf("%s\n",path);
    }
    close(fd);
    break;

  case T_DIR:
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
    //  fprintf(2,"current de name: %s\n",buf);
    //   if(strcmp(fileName,fmtname(de.name)) == 0){
    //     printf("%s\n",de.name);
    //   }
      if(strcmp(de.name,".") != 0 && strcmp(de.name,"..") != 0){
        find_dir(buf,fileName);
      }

    }
    close(fd);
    break;
  }
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    exit(0);
  }
  find_dir(argv[1],argv[2]);
  exit(0);
}
