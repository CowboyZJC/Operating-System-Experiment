#include<stdio.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>

int pid1,pid2;
void kill_process(){
    kill(pid1, 16);
    kill(pid2, 17);
}
void killed(int signum){
    if(signum == 16){
        printf("Child process 1 is killed by parent!\n");
    }
    else if(signum == 17){
        printf("Child process 2 is killed by parent!\n");
    }
}

int main() 
{   
	while((pid1 = fork()) == -1);
	if(pid1>0) 
	{
		while((pid2 = fork()) == -1);
		if(pid2  >  0) 
		{
            signal(2, kill_process);
            signal(14, kill_process);
            alarm(5);
			wait(NULL);
			wait(NULL);
			printf("Parent process is killed!\n");
			exit(0);
		}
		else 
		{
            signal(2,SIG_IGN);
			signal(17, killed);
            pause();
			exit(0);
		}
	}
    else 
    {
        signal(2,SIG_IGN); //子进程中屏蔽ctrl+c的信号
		signal(16, killed);
        pause();
		exit(0);
	}
    return 0;
}