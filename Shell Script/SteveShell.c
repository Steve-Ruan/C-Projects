//
//  SteveShell.c
//  
//
//  Created by steve ruan on 9/18/17.
//
//

#include "SteveShell.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>



#define MAX_LINE		80 /* 80 chars per line, per command */

int main(void)
{
    char input[MAX_LINE];
    char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    int should_run = 1;
    pid_t pid;
    int i;
    
    while (should_run)
    {
        printf("SteveShell>");
        fflush(stdout);
        //read user input
        fgets(input, MAX_LINE, stdin);
        int length = strlen(input);
        if(input[length - 1] == '\n')
        {
            input[length - 1] = '\0';
        }
        if(!strcmp(input, "exit"))
        {
            should_run = 0;
        }
        
        else
        {
        args[0] = strtok(input, " ");
        i = 0;
        while(args[i] != NULL)
        {
            args[++i] = strtok(NULL, " ");
        }
        args[i] = NULL;
        //fork a child process
        pid = fork();
        if(pid==0)
        {
        //child process invokes execvp()
            execvp(args[0], args);
        }
        else
        {
            //parent will invoke wait()
            wait(NULL);
            
        }
            fflush(stdout);
        }

    }
    
    return 0;
}
