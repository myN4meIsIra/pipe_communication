# pipe_communication
Using a slave and master system, create a pipe between two forked processes.

the three parts are completed in the file named to follow the relavent task.  
Ex: sendReply.c holds the code to require a reply to be recieved before sending the next code.  

# sendReply.c         
        on running the program, the following lines will appear:
           - "parent <parentID> running",
           - "parent <parentID>: input a line :"
           - "child <childID> running"
           - "CHECK STATUS CHILD: read ppipe"
                                                 
        these lines of output will appear in two different colors
        all the printf outputs are color-coded for their respective funtion to help me with debugging.
        
        at this point, you should enter an input.  
        
        when you enter an input (or not) and press enter, that input will be edited to have the string "::REPLY" appended to the back of it.
        this is then sent into a different pipe as a signal to the parent function to send the next sting
        
        when this edited string is recieved by the parent process, the process starts again.
        
# timeStamp.c
        There is no "test" copy of this file.  
        On running the program, the following lines will appear:
          - CHECK STATUS MAIN: starting Parent
          - CHECK STATUS MAIN: starting Child
          - CHECK STATUS CHILD  5197 running 
          - CHECK STATUS CHILD: read ppipe  
          - CHECK STATUS PARENT: 5196 running
          - CHECK STATUS PARENT: 5196: input a line : 
                
        this is where you will enter your user input
        
        when you enter an input (or not) and press enter, that input will be edited to have the string ":" appended to the back of it (this helps with ease in tokenizeing the string).
        this is then sent into a different pipe as a signal to the parent function to send the next sting
        
        there will then be several strings containing the difference in hour, minute, second, and milliseconds
        
        the text output will appear in various colours, with each color coinciding to a specific function.
        
        when this edited string is recieved by the parent process, the process starts again.

# timeoutAndSend.c
        There is some type of error in Parent() which I havn't been able to find.
        the logic of what I am trying to do with this part is that, if the child fork doesn't send a response back to the parent through the pipe after 1 second, then an alarm will go off.
        the alarm runs a function which prints to the the user that the string (line) is being re-sent.  It should then call the sendLine() function which writes the string (line) to the pipe.
        However, for whatever reason, despite the printed strings indicating that the logic is followed correctly, the parent doesn't read the re-reply from the child of its re-send.
        
        to use this program:
            - run the executable to run it,
            - type-in some string to print to the pipe
            - the code will be sent to the pipe and read by the child
            - then, it will be edited slightly and sent back to the parent  
