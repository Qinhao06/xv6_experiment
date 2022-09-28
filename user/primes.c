#include "kernel/types.h"
#include "user.h"


void get_prime(int inputNum);

int main(int argc, char* argv[]){
     if(argc != 1){
        printf("too much arg\n"); //检查参数数量是否正确
        exit(-1);
    }
    int pd[2];
    int pid;
    if(pipe(pd) == -1){
        printf("pipe error");
        exit(-1);
    }
    pid = fork();
    if(pid != 0){//father
        close(pd[0]);
        int m = 2;
        write(pd[1], &m, sizeof(int));
        for(int i = 2; i < 36; i++){
           if( i % 2!= 0){
            write(pd[1], &i, sizeof(int));
           }
        }
        close(pd[1]);
    }
    else{//child
        close(pd[1]);
        get_prime(pd[0]);//向后传递
    }
    wait(&pid);  //父进程等待
    exit(0);    
}

void get_prime(int inputNum){
    int prime;
    int m = read(inputNum, &prime, sizeof(int));
    if(m == 0){//m == 0时代表读取完成
        return;
    }
    printf("prime %d\n", prime);
    int pd[2];
    if(pipe(pd) == -1){
        printf("pipe error");
        exit(-1);
    }
    int pid = fork();
    if(pid != 0){
        close(pd[0]);
        int num = 0;
        while (read(inputNum, &num, sizeof(int)))
        {
            if(num % prime != 0){
                write(pd[1], &num, sizeof(int));
            }
        }
        close(pd[1]);
        
    }
    else{//child
        //关闭写管道，进行读取
        close(pd[1]);
        get_prime(pd[0]);
    }
    wait(&pid);
    exit(0);
    
}