//
//  p1.c
//  
//
//  Created by steve ruan on 11/7/17.
//
//


#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h> // For sleep
#define N 10
#define BUFFER_SIZE 5

// Struct for condition variable
struct condition {
    sem_t sem; // Semaphore to be initialized to 0
    int count; // Count of threads waiting
};
sem_t mutex, next;
struct condition not_full;
struct condition not_empty;
int next_count = 0;
int count =0;
int buffer[BUFFER_SIZE];

void cwait(struct condition *c); // Semaphore implementation of conditional wait
void cpost(struct condition *c); // Semaphore implementation of conditional signal
void *add();
void *consume();

int main(int argc, char *argv[]) {
    pthread_t tid1, tid2; // thread identifiers
    not_full.count = 0;
    not_empty.count = 0;
    sem_open(&mutex, 0, 1);
    sem_open(&(not_empty.sem), 0, 0);
    sem_open(&(not_full.sem), 0, 0);
    sem_open(&next, 0, 0);
    
    pthread_create(&tid1, NULL, add, NULL); // Launch add
    pthread_create(&tid2, NULL, consume, NULL); // Launch add
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    sem_destroy(&mutex);
    sem_destroy(&next);
    sem_destroy(&(not_full.sem));
    sem_destroy(&(not_empty.sem));

    
}
void *add() {
    int data[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100}; // Must have N values
    int i, in = 0; // Index for writing
    for(i = 0; i < N; i++) {
    sem_wait(&mutex); //signal mutex
    if(count==BUFFER_SIZE) //if full
    {
        cwait(&not_full); //suspend filling
    }
    

    sleep((double) data[i] / 50); // Not required. Used for testing.
    buffer[in] = data[i]; //buffer index = data
    count++; //increment count
    cpost(&not_empty); //signal to empty
    if (next_count > 0)
        sem_post(&next);
    else
        sem_post(&mutex);
    
    in = (in + 1) % BUFFER_SIZE; //recursively get the new index for buffer

    }
    pthread_exit(0);
}

void *consume() {
    
    int i, out = 0; // Index for reading
    for(i = 0; i < N; i++) {
    sem_wait(&mutex);

    if(count==0)//if buffer is empty
    {
        cwait(&not_empty); //suspend emptying
    }
    int data = buffer[out];
    sleep((double) data / 50); // Not required. Used for testing.
    printf("%d\n", data);
    //fflush(stdout);
    count--;
    cpost(&not_full);
    if (next_count > 0)
        sem_post(&next);
    else
        sem_post(&mutex);
    out = (out + 1) % BUFFER_SIZE;
    }

    pthread_exit(0);
}

// Semaphore implementation of conditional wait
void cwait(struct condition *c) {
    c->count++;
    if (next_count > 0)
        sem_post(&next);
    else
        sem_post(&mutex);
    sem_wait(&(c->sem));
    c->count--;
}

// Semaphore implementation of conditional signal
void cpost(struct condition *c) {
    if (c->count > 0) {
        next_count++;
        sem_post(&(c->sem));
        sem_wait(&next);
        next_count--;
    }
}
