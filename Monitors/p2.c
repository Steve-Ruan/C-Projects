//
//  p2.c
//  
//
//  Created by steve ruan on 11/8/17.
//
//

#include "p2.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#define BUFFER_SIZE 20

struct condition {
    sem_t sem; // Semaphore to be initialized to 0
    int count; // Count of threads waiting
};

sem_t rw_mutex, mutex, next;
char buffer[BUFFER_SIZE];
int buffer_version = 0;
int next_count = 0;
int read_count = 0;
struct condition turn;

void cwait(struct condition *c); // Semaphore implementation of conditional wait
void cpost(struct condition *c); // Semaphore implementation of conditional signal
void *writer();
void *reader();
int main(int argc, char *argv[])
{
    turn.count = 0;
    sem_open(&mutex, 0, 1);
    sem_open(&(turn.sem), 0, 0);
    sem_open(&next, 0, 0);
    pthread_t tid1, tid2; /* the thread identifiers */
    pthread_attr_t attr; /* set of attributes for the thread */
    pthread_attr_init(&attr);
    pthread_create(&tid1, &attr, writer, NULL);
    pthread_create(&tid2, &attr, reader, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    sem_close(&rw_mutex);
    sem_close(&mutex);
}

void *reader()
{
    sem_wait(&mutex);
    cpost(&turn); // Signal for one thread to compute
    cpost(&turn); // Signal for another thread to compute

    int j = 0, old_version = 0;
    pthread_t tid = pthread_self();
    while (old_version != 4) {
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1)
            sem_wait(&rw_mutex);
        sem_post(&mutex);
        if (buffer_version != old_version) {
            printf("%d: %s\n", tid, buffer);
            fflush(stdout); // Not required, but may help
            old_version = buffer_version;
        }
        sem_wait(&mutex);
        read_count--;
        if (read_count == 0)
            sem_post(&rw_mutex);
        sem_post(&mutex);
        
    }
    pthread_exit(0);
}

void *writer()
{
    sem_wait(&mutex);
    cwait(&turn);
    sprintf(buffer, "%s", "Roses are red");
    buffer_version = 1;
    sem_post(&rw_mutex);
    sleep(1);
    sem_wait(&rw_mutex);
    sprintf(buffer, "%s", "Violets are blue");
    buffer_version = 2;
    sem_post(&rw_mutex);
    sleep(1);
    sem_wait(&rw_mutex);
    sprintf(buffer, "%s", "Synchronization is");
    buffer_version = 3;
    sem_post(&rw_mutex);
    sleep(1);
    sem_wait(&rw_mutex);
    sprintf(buffer, "%s", "Quite fun to do");
    buffer_version = 4;
    sem_post(&rw_mutex);
    sleep(1);
    
    if (next_count > 0)
        sem_post(&next);
    else
        sem_post(&mutex);
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
