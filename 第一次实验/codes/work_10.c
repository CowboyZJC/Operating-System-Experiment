#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>


void* threadone(){
    system("./helloworld");
    printf("threadone\n");
    printf("pid = %d\n", syscall(SYS_gettid));
    printf("ppid = %d\n", getppid());
    printf("tid = %d\n", pthread_self());
}

int main(){
    pthread_t id;
    pthread_create(&id, NULL, threadone, NULL);
    pthread_join(id, NULL);
    printf("Main thread\n");
    printf("pid = %d\n", syscall(SYS_gettid));
    printf("ppid = %d\n", getppid());
    printf("tid = %d\n", pthread_self());
    return 0;
}
