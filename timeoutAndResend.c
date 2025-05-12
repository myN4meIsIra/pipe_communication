//

/*
timeoutAndResend.c
Ira Garrett

The child will have a random number generator which will determine if it sends a reply back to the parent. So, reply is not guaranteed, leading to unreceived-data handling
*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>  
#include <sys/time.h>
#include <time.h>

#define LEN 64
int ppipe[2]; //pipe descripttors 
int replyPipe[2];
int pid; //child pid
char line[LEN];
char replyLine[LEN];
time_t time(time_t *tloc);
int close(int fd);
ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int fd, void *buf, size_t count);
struct sigaction new_action,old_action;
struct tm *localtime(const time_t *timep);
pid_t getppid(void);
int parent();
//int setitimer(int which, struct itimerval *value, struct itimerval *ovalue);


//time
//function to find and return current time.
long currentTime()
{
        //define the hour, minute, seconds, and milliseconds
        long millis = clock();
        printf("\033[92mCHECK STATUS CURRENTTIME: millis = %ld\n", millis);
        
        return millis;
}

//function to send the line (called by parent)
void sendLine()
{
        close(ppipe[0]); //parent = pipe writer
                
        //write to the ppipe
        write(ppipe[1], line, LEN); //write to pipe
        printf("\033[94mCHECK STATUS SENDLINE: string %s just sent to ppipe\033[0m\n", line);
        printf("\033[94mCHECK STATUS SENDLINE: parent %d send signal 10 to %d\033[0m\n", getpid(), pid);
       
}

//function to re-send the line (called by the alarm in parent)
void resendLine()
{       //basically just let the user know that the line is being re-sent
        printf("\033[93mCHECK STATUS RESENDLINE: calling sendLine  because the reply hasn't been recieved\033[0m\n");
        sendLine();
}

//funtion to run as the "parent" thread
int parent()
{
        strcpy(replyLine, "");
        //signal that parent has started
        printf("\033[95mparent %d running\n", getpid());

        //infinite loop to write to ppipe and read from replyPipe
        while(1)
        {
                close(ppipe[0]); //parent = pipe writer
                
                //get string to send
                printf("\033[95mparent %d: input a line : \033[0m\n", getpid());
                fgets(line, LEN, stdin);
                line[strlen(line) -1] = 0;  //kill "\n" at the end of the stirng
                printf("\033[95mparent %d write to pipe\033[0m\n", getpid());
                
                //call function to send the line
                sendLine();
                
                while(1)
                {
                        close(replyPipe[1]);
                        printf("\033[95mCHECK STATUS PARENT: recieving...\033[0m\n");

                        new_action.sa_handler = resendLine;
                        sigemptyset(&new_action.sa_mask);   
                        sigaddset(&new_action.sa_mask, SIGALRM);
                        new_action.sa_flags = 0;
                        
                        sigaction(SIGALRM, &new_action, NULL);
                        
                        alarm(1);       
                        read(replyPipe[0], replyLine, LEN);
                        alarm(0);
                        printf("\033[95mCHECK STATUS PARENT: message recieved: %s\033[0m\n",replyLine);
                        if(replyLine != NULL) break;
                }
                       
                                
                
        }
}


//funtion run as the child process
int child()
{
        char msg[LEN];
        int parent = getppid();
        printf("child %d running \n", getpid());
        
        
        //signal(SIGUSR1, chandler);  //install signal catcher
        
        //infinite loop to read ppipe and write relayPipe
        while(1)
        {
                
                close(ppipe[1]);  //child is pipe reader
                printf("\033[37mCHECK STATUS CHILD: reading ppipe and waiting for it to hold something  \033[0m\n");
                
                //read from the ppipe
                read(ppipe[0], line, LEN); //read pipe
                printf("\033[37mCHECK STATUS CHILD: child %d got a message = %s\033[0m\n", getpid(), line);
                
                //copy then append "::REPLY" to the read line
                strcpy(replyLine, line);
                strcat(replyLine, "::REPLY");
                
                printf("\033[37mCHECK STATUS CHILD: send reply: %s\033[0m\n",replyLine);
                close(replyPipe[0]);
                
                //[commented] sleep for 3 seconds to make sure that parent waits for the signal from replyPipe
                //sleep(3);
                
                //write the edited line to the replyPipe


                int rInt = rand() % 2;      // Returns a pseudo-random integer between 0 and RAND_MAX.
                printf("\033[37mCHECK STATUS CHILD:random int = %d \033[0m\n",rInt);
                if(rInt == 1)
                {
                        printf("\033[37mCHECK STATUS CHILD: the child will now send the reply because the random int = %d \033[0m\n",rInt);
                        write(replyPipe[1], replyLine, LEN);
                        printf("\033[37mCHECK STATUS CHILD: replyLine sent: replyLine=%s \033[0m\n",replyLine);
                }
                else
                {
                        printf("\033[37mCHECK STATUS CHILD: will not send reply because the random int = %d \033[0m\n",rInt);
                }
        }
}


//main function
int main ()
{

        //srand(time(NULL));   // Initialization, should only be called once.
        
        //create the two pipes
        pipe(ppipe); // create the pipe
        pipe(replyPipe);
        
        pid = fork(); // fork a child process
        
        if(pid) 
                // parent
                parent();
        else
                //chile
                child();

        //return -1 in case of failure
        return -1;
}

