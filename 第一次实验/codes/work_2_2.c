#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(){
pid_t pid, pid1;
    pid = fork();
    if(pid < 0){
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0){
        pid1 = getpid();
        printf("child: pid = %d", pid);   // A
        printf("child: pid1 = %d", pid1);  // B
        int i,j = 0;
        for(i=0;i<1000;i++){
            j++;
        }
    }
    else{
        pid1 = getpid();
        printf("parent: pid = %d", pid);   // C 
        printf("parent: pid1 = %d", pid1);   // D
    }
    printf("ppid = %d", getppid());
    return 0;
}