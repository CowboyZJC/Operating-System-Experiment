#include  <unistd.h>                                                                                                       
#include  <signal.h>                                                                                                       
#include  <stdio.h>
#include  <stdlib.h>                                                                                                        
int pid1,pid2;     // 定义两个进程变量 
int main(){
    int fd[2];
    int i = 0; 
    char InPipe[10000];  	     // 定义读缓冲区
    char c1='1', c2='2'; 
    pipe(fd);                   			// 创建管道 
    while((pid1 = fork( )) == -1);  	// 如果进程1创建不成功,则空循环 
    if(pid1 == 0) {               // 如果子进程1创建成功,pid1为进程号 
        lockf(fd[1],1,0);			// 锁定管道 
        for(i=0;i<2000;i++){
            write(fd[1],&c1,1);     //  分2000次每次向管道写入字符’1’
        }			 
        sleep(5);               			// 等待write读出数据 
        lockf(fd[1],0,0);       			// 解除管道的锁定 
        exit(0);                 		// 结束进程1 
    } 
    else{ 
        while((pid2 = fork()) == -1);          		// 若进程2创建不成功,则空循环 
        if(pid2 == 0) { 
            lockf(fd[1],1,0); 
            for(i=0;i<2000;i++){
            write(fd[1],&c2,1);         //  分2000次每次向管道写入字符’2’
            } 			                
            sleep(5);   //等待write
            lockf(fd[1],0,0); 
            exit(0); 
        } 
        else{ 
            //wait(0);                          		// 等待子进程1 结束 
            //wait(0);                      			// 等待子进程2 结束 
            lockf(fd[0],1,0);         			// 从管道中读出4000个字符
            read(fd[0], InPipe, 4000);
            InPipe[4000] = '\0'; 
            lockf(fd[0],0,0);                                                             //  加字符串结束符 
            printf("%s\n",InPipe);         			// 显示读出的数据
            exit(0);                        				// 父进程结束  
        }
    } 
}