#include <stdio.h> 
#include <signal.h>     
#include <unistd.h>  
#include <sys/types.h>
#include <stdlib.h>
int wait_flag; 
void stop(int signum); 
int main(){ 
    int pid1, pid2;     					  
    signal(2,stop);           
    signal(14,stop); 
    alarm(5);				
    while((pid1 = fork( )) == -1);		
    if(pid1 > 0) {             	  	 
        while((pid2 = fork( )) == -1);	     
            if(pid2 > 0) { 
                wait_flag = 1; 
                //alarm(5);
                while(wait_flag);          				      			    
                printf("\nkill!\n"); 
                wait(0);           			 
                wait(0);           			
                printf("\n Parent process is killed !!\n"); 
                exit(0);           				           
            } 
            else { 
                signal(2,SIG_IGN);
                wait_flag = 1;                        
                while(wait_flag);                                                                                   
                printf("\n Child process 2 is killed by parent !!\n"); 
                exit(0);                                                                                                    
            }                                                                                                                                                                
    }                                                                                                                    
    else {
        signal(2,SIG_IGN);                                                                                                               
        wait_flag = 1;                                                                                                   
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