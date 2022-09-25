#include "kernel/types.h"
#include "user.h"


void get_prime(int inputNum);

int main(int argc, char* argv[]){
     if(argc != 2){
        printf(" more or less args\n"); //检查参数数量是否正确
        exit(-1);
    }
    int pd[2];
    int pid;
    if(pipe(pd) == -1){
        printf("pipe error");
        exit(-1);
    }
    pid = fork();
    int n =atoi(argv[1]);//传入参数为字符串
    if(pid != 0){//father
        close(pd[0]);
        for(int i = 2; i < n; i++){
            write(pd[1], &i, sizeof(int));
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
/**
 * 用于实现创建子进程和通过管道传递数据
 * @param inputNum 管道标识
 * 
*/
void get_prime(int inputNum){
    int prime;
    int pd[2];
    if(pipe(pd) == -1){
        printf("pipe error");
        exit(-1);
    }
    //读取数据
    int m = read(inputNum, &prime, sizeof(int));
    if(m == 0){//c == 0时代表读取完成
        return;
    }
    printf("prime:%d\n", prime);
    
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
        close(pd[1]);
        get_prime(pd[0]);
    }
    wait(&pid);
    exit(0);
    
}