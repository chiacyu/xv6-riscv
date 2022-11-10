#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void find_file(char *filepath, char *key)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(filepath, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", filepath);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", filepath);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    printf("%s\n", de.name);
    break;

  case T_DIR:
    if(strlen(filepath) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, filepath);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || (strcmp(de.name, ".")==0) || (strcmp(de.name, "..")==0))
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      if(st.type == T_DIR){
        find_file(de.name, key);
      }
      if(st.type == T_FILE){
        if (strcmp(de.name, key)==0)
        {
            printf("%s\n", de.name);
        }
      }
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
    if( argc < 3){
        fprintf(2, "Usage: Find <filepath> <key>\n");
    }    
    find_file(argv[1], argv[2]);
    exit(0);
}