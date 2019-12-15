#include <stdio.h>
#include <pthread.h>

#define MAX_THREADS 2

void *work_thread(void *arg){
    printf("Thread %d: is working...\n", arg);
    return arg;
}
int main() {
    pthread_t tid[MAX_THREADS];
    int i, status;
    for(i = 0; i < MAX_THREADS; i++){
        status = pthread_create(&tid[i], NULL, work_thread, (void *)i);
        if(status != 0){
            perror("Create thread");
        }
    }
    pthread_exit(NULL);
    return 0;
}
