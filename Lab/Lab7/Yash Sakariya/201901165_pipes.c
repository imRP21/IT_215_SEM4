#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

int main()
{
    int MSGLEN=100;

    //Array of Questions
    char* questions[] = {"Quit", "In which university do you study?","Which course are you studying?", "What is your area of interest?"};
    
    //Printing The Questions
    for(int i=0;i<=3;i++)
        printf("Question %d : %s\n", i, questions[i]);

    //Creating file pointer
    FILE * fPtr;
	fPtr = fopen("201901302_pipes.txt", "w");
	fclose(fPtr);

    while(1)
    {
        //Two pipes, fd1 for Parent Process and fd2 for Child Process
        int fd1[2];
        int fd2[2];

        int r1 = pipe(fd1);
        int r2 = pipe(fd2);
        //Gives error if Pipe is not created successfully
        if(r1 == -1 || r2 == -1)
        {
            fprintf(stderr, "Pipe Operation Failed" );
            return 1;
        }

        //Pointer to the file is stored in oldfd. "O_WRONLY | O_APPEND" - to open the file in write and append mode.
        int oldfd = open("201901165_pipes.txt", O_WRONLY | O_APPEND);
		if(oldfd < 0)
            printf("Error opening the file\n");

       if(!fork())   //Child Process
       {
            close(fd1[WRITE]);
            //Answer arrar
            char* answers[] = {"Quit", "DAIICT", "Systems Software", "Kernel Programming"};
            char message[100];

            //Child Process reads the question received from the Parent process i.e., fd1
            int bytesRead = read(fd1[READ], message, MSGLEN);

            if(!strcmp(message, answers[0])) //If "Quit"
            {
                close(fd2[READ]);
                write(fd2[WRITE], answers[0], strlen(answers[0]) + 1);
                close(fd1[READ]);
                close(fd2[WRITE]);
            }
            else
            {
                for(int i = 0;i <= 3; i++)
                {
                    if(!strcmp(questions[i], message))
                    {
                        //Child Process writes the answer of received question in fd2
                        write(fd2[WRITE], answers[i], strlen(answers[i]) + 1);

                        //Appending in the file
                        char *newstr = malloc(strlen(message) + 2);
                        strcpy(newstr, message);
                        strcat(newstr, "\n");
                        write(oldfd, newstr, strlen(newstr));
                        break;
                    }
                }
                close(fd2[WRITE]);
            }
            exit(0);
       }
       else
       {
            close(fd1[READ]); //close read for Parent Pipe i.e., fd1
            char message[100];  //String for storing answer.
            int que;   //Question number

            printf("Enter question number: ");
            scanf("%d",&que);

            if(que<0 || que>3)
            {
                printf("Invalid Question Number.\n");
                continue;
            }

            //Writing in the Parent pipe.
            write(fd1[WRITE],questions[que],strlen(questions[que])+1);

            //Waiting for the Child process to complete exection
            wait(NULL);
            close(fd2[WRITE]);

            //Reading the answer received from the Child Process
            int bytesRead=read(fd2[READ],message,MSGLEN);

            printf("Answer: %s\n",message);
            if(!strcmp(message,"Quit"))   //If "Quit"
            {
                close(fd2[READ]);
                close(fd2[WRITE]);
                close(fd1[READ]);
                close(fd1[WRITE]);
                close(oldfd);
                break;
            }
            close(fd2[READ]);

            strcat(message, "\n");
            //Appending in the file.
            write(oldfd,message,strlen(message));
        }
    }
    return 0;
}
