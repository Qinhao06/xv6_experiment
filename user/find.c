#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
/**
 * @param path：待处理字符串
 * @return char* :当前路径最后一个文件名称
 * 
 * 
*/
char*
getname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
 return p;
}


/**
 * @param destination:目标文件
 * @param path：当前查找路径
 * @return null
*/
void find(char *destination, char *path){
 static char buf[512];
  char *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  //根据文件类型选择不同的处理方式，文件夹递归进入
  switch(st.type){
  case T_FILE:
    if(strcmp(fmtname(path), destination) == 0){
        printf("%s\n", buf);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      if(strcmp(destination, getname(buf)) == 0){
        printf("%s\n", buf);
      }

      if(strcmp(".", getname(buf)) != 0 && strcmp("..", getname(buf)) != 0){
        find(destination, buf);
      }
      fmtname(buf);
      
    }
    break;
  }
  close(fd);
    
}

int 
main(int argc, char *argv[]){
    if(argc == 2){
      find(argv[1], ".");
      exit(0);
    }
    
    if(argc == 3){
      find(argv[2], argv[1]);
      exit(0);
    }
    printf("arg err\n");
    exit(-1);
}