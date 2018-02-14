//
//  p2.c
//  
//
//  Created by steve ruan on 10/25/17.
//
//

#include "p2.h"
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

#define BUFFER_SIZE 10

char buffer[BUFFER_SIZE] ={};

int version = 0;
sem_t rw_mutex;
sem_t mutex;
int read_count = 0;
void *consumer();

int main(int argc, char *argv[])
{

    if(sem_open(&rw_mutex, 0, 1) < 0) { // 0 = multithreaded
        fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
        exit(0);
    }
    
    if(sem_open(&mutex, 0, 1) < 0) { // 0 = multithreaded
        fprintf(stderr, "ERROR: could not initialize &semaphore.\n");
        exit(0);
    }
    pthread_t tid1; /* the thread identifier */
    pthread_t tid2; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */
    pthread_attr_init(&attr);
    pthread_create(&tid1, &attr, consumer, NULL);
    pthread_create(&tid2, &attr, consumer, NULL);
    //string chars
    char *s1 = "Roses are read";
    char *s2 = "Violets are blue";
    char *s3 = "Synchronization is";
    char *s4 = "Quite fun to do!";
    
    //parent is the writer
    //write first buffer
    sem_wait(&rw_mutex);
    sprintf(buffer, "%s", s1);
    version++;
    sem_post(&rw_mutex);
    sleep(1);
    //write second buffer
    sem_wait(&rw_mutex);
    sprintf(buffer, "%s", s2);
    version++;
    sem_post(&rw_mutex);
    sleep(1);
    //write third buffer
    sem_wait(&rw_mutex);
    sprintf(buffer, "%s", s3);
    version++;
    sem_post(&rw_mutex);
    sleep(1);
    //write fourth buffer
    sem_wait(&rw_mutex);
    sprintf(buffer, "%s", s4);
    version++;
    sem_post(&rw_mutex);
    sleep(1);
    //join the processes
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);
    
    
}

void *consumer()
{
    pthread_t tid = pthread_self();
    int old = 0;
        while (old != 4) {
            sem_wait(&mutex);
            read_count++;
            if (read_count == 1) {
                sem_wait(&rw_mutex);
            }
            sem_post(&mutex);
            //print thread ID and buffer when buffer has changed (aka version)
            if (old != version) {
                printf("%d: %s\n", tid, buffer);
                fflush(stdout);
                old = version;
            }
            sem_wait(&mutex);
            read_count--;
            if (read_count == 0) {
                sem_post(&rw_mutex);
            }
            sem_post(&mutex);
        }

        pthread_exit(0);
    }
