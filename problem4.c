#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 5
#define BUF_SIZE 512

sigset_t sigset;

typedef struct{
    char msg[BUF_SIZE];
    int wr_check;
    pthread_mutex_t mutex;
    pthread_cond_t read;
}buffer;

int thread_flag[MAX_THREADS] = {0,};

buffer bb = {"no message", 0,
             PTHREAD_MUTEX_INITIALIZER,
             PTHREAD_COND_INITIALIZER};

int input_msg(){
    int status, signal, i;

    while(1){
        sigwait(&sigset, &signal);
        if(signal == SIGTSTP){
            status = pthread_mutex_lock(&bb.mutex);
            if(status != 0){
                return status;
            }
            printf("input : ");
            fgets(bb.msg, BUF_SIZE, stdin);
            bb.wr_check = 1;

            for(i = 0; i < MAX_THREADS; i++){
                thread_flag[i] = 1;
            }

            if(pthread_cond_signal(&bb.read)){
               pthread_mutex_unlock(&bb.mutex);
               return status;
            }
            pthread_mutex_unlock(&bb.mutex);
        }
    }
}

int print_msg(void *arg){
    int status;

    status = pthread_mutex_lock(&bb.mutex);
    if(status != 0){
        return status;
    }
    status = pthread_cond_wait(&bb.read, &bb.mutex);
    if(status != 0){
        pthread_mutex_unlock(&bb.mutex);
        return status;
    }
    if(bb.wr_check && thread_flag[(int)arg]) {
        printf("Thread %d : %s", (int)arg, bb.msg);
        pthread_cond_signal(&bb.read);
        thread_flag[(int)arg] = 0;
    }
    pthread_mutex_unlock(&bb.mutex);
    //sleep(1);
    return 0;
}

void *signal_thread(void *arg){
    while(1) {
        input_msg();
    }
}

void *msg_thread(void *arg){
    while(1){
        print_msg(arg);
    }
}

int main(){
    int i, status;
    int *result;
    pthread_t tid[MAX_THREADS];

    sigemptyset(&sigset);
    sigaddset(&sigset, SIGTSTP);
    status = pthread_sigmask (SIG_BLOCK, &sigset, NULL);
    if (status != 0){
        fprintf (stderr, "Set signal mask");
        exit(1);
    }
    for(i = 0; i < MAX_THREADS; i++){
        if( i == 0) {
            status = pthread_create(&tid[i], NULL, signal_thread, NULL);
            if(status != 0){
                perror("create thread");
                exit(1);
            }
        }else{
            status = pthread_create(&tid[i], NULL, msg_thread, (void *)i);
            if(status != 0){
                perror("create thread");
                exit(1);
            }
        }
    }
    pthread_exit(result);
}