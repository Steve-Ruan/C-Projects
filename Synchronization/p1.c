#include <stdio.h>
#include <math.h>
#define BUFFER_SIZE 10
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


#define N 10

int buffer[BUFFER_SIZE] = {0, 0, 0, 0, 0};
int values[N] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
int counter = 0;
int in = 0;
int out = 0;
sem_t full, mutex_lock, empty;

void *producer();
void *consumer();

int main(int argc, char *argv[])
{
    
    if(sem_open(&full, 0, 0) < 0) { // 0 = multithreaded
        fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
        exit(0);
    }
    
    if(sem_open(&mutex_lock, 0, 1) < 0) { // 0 = multithreaded
        fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
        exit(0);
    }
    
    if(sem_open(&empty, 0, 5) < 0) { // 0 = multithreaded
        fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
        exit(0);
    }
    
    pthread_t tid1; /* the thread identifier */
    pthread_t tid2; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */
    pthread_attr_init(&attr);
    pthread_create(&tid1, &attr, producer, NULL);
    pthread_create(&tid2, &attr, consumer, NULL);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
}

void *producer() {
    while (counter < BUFFER_SIZE) {
        sem_wait(&empty);
        sem_wait(&mutex_lock);
        buffer[in] = values[counter];
        in = (in + 1) % BUFFER_SIZE;
        counter++;
        sem_post(&mutex_lock);
        sem_post(&full);
    }
}

void *consumer()
{
    int next;
    while (counter > 0)
    {
        sem_wait(&full);
        sem_wait(&mutex_lock);
        next = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        counter--;
        sem_post(&mutex_lock);
        sem_post(&empty);
        printf("Consumed: %d \n", next);
    } 
}


