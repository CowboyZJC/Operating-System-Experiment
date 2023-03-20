#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>


pthread_mutex_t mtx;
void* threadone(int* i){
     int j;
     pthread_mutex_lock(&mtx);
     for(j=0; j<5000; j++){
        (*i)++;
     }
     printf("i = %d\n", *i);
     pthread_mutex_unlock(&mtx);
}
int main(){
    pthread_mutex_init(&mtx, NULL);
    pthread_t id;
    int i;
    i = 0;
    int j;
    pthread_create(&id, NULL, (void*)threadone, &i);
    pthread_mutex_lock(&mtx);
    for(j=0; j<5000; j++){
        i--;
    }
    printf("i = %d\n", i);
    pthread_mutex_unlock(&mtx);
    pthread_join(id, NULL);
    pthread_mutex_destroy(&mtx);
    return 0;
}