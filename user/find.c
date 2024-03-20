#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"


char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}


void
find(char *path, char *file_name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    char* name = fmtname(path);
    // printf("%s\n", file_name);
    // printf("%s\n", name);
    // printf("%d\n", strlen(name));
    // printf("%d\n", strlen(file_name));
    if (strcmp(name, file_name) == 0)
    {
        printf("%s\n", path);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      {
        continue;
      }
    //   printf("%s\n", de.name);
    //   printf("%d\n", strlen(de.name));
      memmove(p, de.name, DIRSIZ);
      p[1 + strlen(de.name) + 1] = 0;
      find(buf, file_name);
    }
    break;
  }
  close(fd);
}


int
main(int argc, char *argv[]) {
//   printf("%s\n", argv[1]);
//   printf("%s\n", argv[2]);
  find(argv[1], argv[2]);
  exit(0);  
}