//
//  HW3Q2.c
//  
//
//  Created by steve ruan on 9/19/17.
//
//

#include "HW3Q2.h"
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#define N 1000000000

int main()
{
    int answer;
    printf("How many processes: ");
    scanf("%d", &answer);
    if(answer ==0)
    {
        printf("Null");
    }
    else if(answer==1)
    {
        long int i;
        long long int x = 0;
        struct timeval start_time, stop_time, elapsed_time;  // timers
        gettimeofday(&start_time,NULL); // Unix timer
        
        for (i = 0; i < N; i++) {
            x = x + 1;
        }
        
        gettimeofday(&stop_time,NULL);
        timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
        printf("Process 1 total time was %f seconds. sum = %lld\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
    }
    else if(answer == 2)
    {
        long int i;
        long long int x = 0;
        pid_t pid;
        struct timeval start_time, stop_time, elapsed_time;  // timers
        gettimeofday(&start_time,NULL); // Unix timer
        pid = fork();
        if (pid > 0) {
            for (i = 0; i < N/2; i++) {
                x = x + 1;
            }
            gettimeofday(&stop_time,NULL);
            timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
            printf("Process 1 total time was %f seconds. sum = %lld.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
        }
        else if (pid == 0) { /* child process */
            for (i = 0; i < N/2; i++) {
                x = x + 1;
            }
            gettimeofday(&stop_time,NULL);
            timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
            printf("Process 2 total time was %f seconds. sum = %lld.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
        }
    }
    else if(answer==4)
    {
        int processid = 1;
        long int i;
        long long int x = 0;
        pid_t pid;
        struct timeval start_time, stop_time, elapsed_time;  // timers
        gettimeofday(&start_time,NULL); // Unix timer
        pid = fork();
        if (pid > 0) {
            pid = fork();
            if (pid > 0)
            {
                pid = fork();
                if(pid==0)
                    processid = 4;
            }
            else
            {
                processid = 3;
            }
        }
        else
        {
            processid = 2;
        }
        
        for (i = 0; i < N/4; i++) {
            x = x + 1;
        }
        wait(NULL);
        gettimeofday(&stop_time,NULL);
        timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
        printf("Process %d total time was %f seconds. x = %lld.\n", processid ,elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);
        fflush(stdout);
    }
    else if(answer == 3)
    {
        int processid = 1;
        long int i;
        long long int x = 0;
        pid_t pid;
        struct timeval start_time, stop_time, elapsed_time;  // timers
        gettimeofday(&start_time,NULL); // Unix timer
        pid = fork();
            if (pid > 0)
            {
                pid = fork();
                if(pid==0)
                    processid = 3;
            }
            else
            {
                processid = 2;
            }

        
        for (i = 0; i < N/3; i++) {
            x = x + 1;
        }
        wait(NULL);
        gettimeofday(&stop_time,NULL);
        timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
        printf("Process %d total time was %f seconds. x = %lld.\n", processid ,elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0, x);

    }
    else
    {
        printf("Invalid");
    }
}
