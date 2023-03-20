#include<stdio.h>
#include <unistd.h>
int main(){
    printf("hello world!\n");
    printf("helloworld的pid = %d\n", getpid());
    printf("helloworld的ppid = %d\n", getppid());
    return 0;
}