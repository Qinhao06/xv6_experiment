#include "kernel/types.h"
#include "user.h"

int main(int argc, char* argv[]){
    if(argc != 1){
        printf("too much arg\n"); //检查参数数量是否正确
        exit(-1);
    }
    int pd_1[2];
    int pd_2[2];
    int p1 = pipe(pd_1);
    int p2 = pipe(pd_2);
    char buff[4];
    if(p1 == -1 || p2==-1){
        printf("pipe error");
        exit(-1);
    }
    int pid_t = fork();

    if(pid_t == -1){
        printf("fork failed");
        exit(-1);
    }
    else if(pid_t == 0){//子进程
        //先执行读，采用双向管道
        close(pd_2[1]);//要先释放不使用的管道口
        close(pd_1[0]);
        read(pd_2[0], buff, 4);
        if(strcmp(buff, "ping") == 0){
            printf("%d: received %s\n", getpid(), buff);
        }
        close(pd_2[0]);
        write(pd_1[1], "pong", 4);
        close(pd_1[1]);
        
    }
    else { //父进程
        close(pd_2[0]);
        close(pd_1[1]);
        write(pd_2[1], "ping", 4);
        close(pd_2[1]);
        read(pd_1[0], buff, 4);
        if(strcmp(buff, "pong") == 0){
            printf("%d: received %s\n", getpid(), buff);
        }
        close(pd_1[0]);
    }
    //printf("hello");
    exit(0);
}



