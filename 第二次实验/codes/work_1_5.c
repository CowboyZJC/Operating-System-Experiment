#include <stdio.h> 
#include <signal.h>     
#include <unistd.h>  
#include <sys/types.h>
#include <stdlib.h>
int wait_flag; 
void stop(int signum); 
int main(){ 
    int pid1, pid2;     					// 定义两个进程号变量  
    signal(3,stop);            				// 或者 signal(14,stop); 
    while((pid1 = fork( )) == -1);		// 若创建子进程1不成功,则空循环
    if(pid1 > 0) {             	  	// 子进程创建成功,pid1为进程号 
        while((pid2 = fork( )) == -1);	// 创建子进程2      
            if(pid2 > 0) { 
                wait_flag = 1; 
                sleep(5);          				
                printf("\nkill 1!\n"); 
                kill(pid1,16);      		       // 杀死进程1
                wait(0);           			// 等待第1个子进程1结束的信号 
                printf("\nkill 2!\n"); 
                kill(pid2,17);       			    // 杀死进程2 
                wait(0);           			// 等待第2个子进程2结束的信号 
                printf("\n Parent process is killed !!\n"); 
                exit(0);           				           // 父进程结束 
            } 
            else { 
                signal(3,SIG_IGN);
                wait_flag = 1; 
    		    signal(17,stop);                           // 等待进程2被杀死的中断号17
                while(wait_flag);                                                                                   
                printf("\n Child process 2 is killed by parent !!\n"); 
                exit(0);                                                                                                    
            }                                                                                                                                                                
    }                                                                                                                    
    else {
        signal(3,SIG_IGN);                                                                                                               
        wait_flag = 1;                                                                                                  
		signal(16,stop); 	// 等待进程1被杀死的中断号16  
        while(wait_flag);                                                                                     
        printf("\n Child process 1 is killed by parent !!\n");                                                        
        exit(0); 
    }                   
    return 0;                                                                                                  
}   
void stop(int signum){ 
    wait_flag = 0;         
    printf("\n %d stop \n",signum);                                                                                                   
} 