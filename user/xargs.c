#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

/*
 *类似于fget函数：读取一行数据
 @param int fd 文件描述符
 @param char*buf 读入位置 
 @return : 0--false 1--success
*/

int fget_line(int fd, char*buf){
    int i = 0;
    char c;
    int r = read(fd, &c, 1);
    while (r == 1)
    {
        
        if(c == '\n'){
            return 1;
        }
        else{
            *(buf + i) = c;
            i++;   
            if(i > DIRSIZ){
                printf("input too long");
                return 0;
            }
        }
        r = read(fd, &c, 1);
    }
    return 0;
    
}

int main(int argc, char *argv[]){
    char *exc_arg[argc + 2];
    int i;
    //xargs参数获取
    for ( i = 0; i < argc - 1; i++)
    {
       
        if(strcmp(strchr(argv[i+1], '.'), argv[i + 1]) == 0){
            exc_arg[i] = argv[i+1] + 2;
        }
        else{
             exc_arg[i] = argv[i + 1];
        }
        //printf("%s\n",exc_arg[i]);
    }

    //为后面参数存储分配空间
    for(int j = i; j < argc + 2; j++){
        exc_arg[j] = (char*)malloc(sizeof(char) * DIRSIZ);
    }

    //从标准输入读入数据
    int fd = dup(0);
    char buf[DIRSIZ];
    int flag = 1;

    //buf作为标准输入的缓冲区，将exc_arg[i]指向buf。
    exc_arg[i] = buf;
    *exc_arg[i + 1] = 0;
    while(flag == 1){
        
        //更新缓冲区
        memset(buf, 0, DIRSIZ);
        flag = fget_line(fd, buf);

        //创建子进程执行命令
        int pid = fork();
        if(flag == 1 && pid == 0){
           int b = exec(exc_arg[0], exc_arg);
            if(b  == -1){
                printf("%s err \n", argv[1]);
                exit(-1);
                }
        }
        else{

            wait(&pid);

        }
    }
    exit(0);
    
    
   
}
