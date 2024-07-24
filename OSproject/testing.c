#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>


#define SIGHUP  1   /* Hangup the process */ 
#define SIGINT  2   /* Interrupt the process */ 
#define SIGQUIT 3   /* Quit the process */ 
#define SIGILL  4   /* Illegal instruction. */ 
#define SIGTRAP 5   /* Trace trap. */ 
#define SIGABRT 6   /* Abort. */

void handle_sigint(int a)
{
	printf("Terminated with signal %d\n",a);
	exit(1);
}


int main()
{
	signal(SIGINT, handle_sigint); 
    while (1) 
    { 
        printf("hello world\n"); 
        sleep(1); 
    } 
    return 0; 
} 
