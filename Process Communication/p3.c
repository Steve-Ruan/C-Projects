//
//  p3.c
//  
//
//  Created by steve ruan on 10/2/17.
//
//

#include "p3.h"
#include <pthread.h>
#include <stdio.h>
#include <math.h>

int sum1, sum2;
double x1, x2, y, min = 0, min2 = 0;
void *child1(); /* the thread */
void *child2();

int main(int argc, char *argv[])
{
    pthread_t tid1; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */
    
    /* get the default attributes */
    pthread_attr_init(&attr);
    
    /* create the threads */
    pthread_create(&tid1,&attr,child1,NULL);
    for (x1 = -2; x1 <= 0; x1 += 0.5) {
        for (x2 = -2; x2 <= 2; x2 += 0.5) {
            y = shubert(x1,x2);
            if(y<min)
                min=y;
        }
    }
    /* now wait for the thread to exit */
    pthread_join(tid1,NULL);
    if (min2 > min) {
        printf("min = %.5f\n", min);
    }
    else {
        printf("min = %.5f\n", min2);
    }

    
}

/**
 * Child threads will begin control in one of these functions
 */
       void *child1()
    {
        int i;
        sum1 = 0;
        for (i = 1; i <= 10; i++)
            sum1 += i;
        pthread_exit(0);
    }

void *child2()
{
    for (x1 = .01; x1 <= 2; x1 += 0.5) {
        for (x2 = -2; x2 <= 2; x2 += 0.5) {
            y = shubert(x1, x2);
            if (y < min2)
                min2 = y;
        }
    }
    //printf("Child 2 min = %.2f \n", min);
    pthread_exit(0);
}
shubert(double x1, double x2) {
    double sum1 = 0;
    double sum2 = 0;
    int i;
    for (i = 1; i <= 5; i++) {
        sum1 += i * cos((i + 1) * x1 + i);
        sum2 += i * cos((i + 1) * x2 + i);
    }
    return sum1 * sum2;
}
