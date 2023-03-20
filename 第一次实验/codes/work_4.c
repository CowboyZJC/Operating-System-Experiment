#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(){
int i = 0;   //定义全局变量
pid_t pid, pid1;
    pid = fork();
    if(pid < 0){
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0){
        i++;   
        printf("i = %d\n", i);
        printf("i的虚拟地址是%hX\n", &i);
        pid1 = getpid();
        printf("child: pid = %d\n", pid);   // A
        printf("child: pid1 = %d\n", pid1);  // B
    }
    else{      
        i--;
        printf("i = %d\n", i);
        printf("i的虚拟地址是%hX\n", &i);
        pid1 = getpid();
        printf("parent: pid = %d\n", pid);   // C 
        printf("parent: pid1 = %d\n", pid1);   // D
        wait(NULL);

    }
    i--;   
    printf("return前的i = %d\n", i);
    return 0;
}