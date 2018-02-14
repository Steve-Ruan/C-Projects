//
//  HW2.c
//  
//
//  Created by steve ruan on 9/12/17.
//
//

#include "HW2.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    pid = fork();
    
    if(pid >0)
    {
        printf("Process 1: %d, Parent: %d\n", getpid(), getppid());
        pid = fork();
        if(pid>0)
        {
            sleep(2);
        }
        else if(pid==0)
        {
            printf("Process 3: %d, Parent: %d \n", getpid(), getppid());
            
        }
        
    }
    else
    {
        
        printf("Process 2: %d, Parent: %d \n", getpid(), getppid());
    }
    
}
