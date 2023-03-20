#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>


void* threadone(int* i){
     int j;
     for(j=0; j<5000; j++){
        (*i)++;
     }
     printf("i = %d\n", *i);
}
int main(){
    pthread_t id;
    int i;
    i = 0;
    int j;
    pthread_create(&id, NULL, (void*)threadone, &i);
    pthread_join(id, NULL);
    for(j=0; j<5000; j++){
        i--;
    }
    printf("i = %d\n", i);
    return 0;
}












