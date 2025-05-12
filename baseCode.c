// base code
/*
Ira Garrett
*/

#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>  

#define LEN 64
int ppipe[2]; //pipe descripttors 
int pid; //child pid
char line[LEN];

int close(int fd);
ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int fd, void *buf, size_t count);
pid_t getppid(void);





int parent()
{
        printf("parent %d running\n", getpid());
        close(ppipe[0]); //parent = pipe writer
        while(1)
        {
                printf("parent %d: input a line : \n", getpid());
                fgets(line, LEN, stdin);
                line[strlen(line) -1] = 0;  //kill "\n" at the end of the stirng
                printf("parent %d write to pipe\n", getpid());
                write(ppipe[1], line, LEN); //write to pipe
                printf("parent %d send signal 10 to %d\n", getpid(), pid);
                kill(pid, SIGUSR1); //send signal to child process
        }
}


void chandler(int sig)
{
        printf("\nchild %d got an interrupt sig=%d\n", getpid(), sig);
        read(ppipe[0], line, LEN); //read pipe
        printf("child %d get a message = %s\n", getpid(), line);
}


int child()
{
        char msg[LEN];
        int parent = getppid();
        printf("child %d running \n", getpid());
        close(ppipe[1]);  //child is pipe reader
        signal(SIGUSR1, chandler);  //install signal catcher
        while(1);
        
}


int main ()
{
        pipe(ppipe); // create the pipe
        pid = fork(); // fork a child process
        if(pid) // parent
                parent();
        else
                child();

        return -1;
}

