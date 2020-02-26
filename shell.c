#include <fcntl.h>
#include<sys/wait.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//gen cohen
//fiore operating systems 2020 spring
//lab 2
//making a shell

int handleArray(char **ptr);
int findOutPlace(char **argv);
int checkRedirect(char **ptr);//send it the array of words, it checks if there is output redirection of > or >>, returns 1 >, 2 for >>
int main(int argc2, char *argv2[])
{

	int running =1;//set to 0 when want to stop





	//main program loop
	while(running ==1)
	{

		if(argv2[1] != NULL)//if they gave a batch file
		{
			printf("you gave a batch file: %s\n", argv2[1]);

			//get their input

  			char *line_buf = NULL;
  			size_t line_buf_size = 0;
  			int line_count = 0;
  			ssize_t line_size;
  			FILE *fp = fopen(argv2[1], "r");
  			if (!fp)
  			{
   	 			fprintf(stderr, "Error opening file '%s'\n", argv2[1]);
    				return EXIT_FAILURE;
  			}

  			/* Get the first line of the file. */
  			line_size = getline(&line_buf, &line_buf_size, fp);

  			/* Loop through until we are done with the file. */
  			while (line_size >= 0)
  			{

				printf("\nI read in %s\n", line_buf);
    				/* Increment our line count */
    				line_count++;

    				/* Show the line details */
    				//printf("chars=%06zd, contents: %s", line_size, line_buf);

				char* argv[100];
                		int argc =1;
                		int i =0;

                		//seperate the input by spaces
                		char delim[] = " ";
                		char *token;

                		/* get the first token */
                		token = strtok(line_buf, delim);

                		/* walk through other tokens */
                		while( token != NULL ) {
                        		token[strcspn(token, "\n")] = 0;
                        		argv[i++] = token;
                        		//printf( "token in loop:  %s\n", token );
                        		argc +=1;

                        		token = strtok(NULL, delim);
                		}//end reading string into array
                		argv[i++] = NULL;//Set the first location in the array after your C-Strings to NULL.


				running = handleArray(argv);

				/* Get the next line */
    				line_size = getline(&line_buf, &line_buf_size, fp);
  			}//end main program loop for batchfile

  			/* Free the allocated line buffer */
  			free(line_buf);
  			line_buf = NULL;

  			/* Close the file now that we are done with it */
  			fclose(fp);


		}//end if batch file
		else
		{

			printf("myshell> ");//print the prompt

			//get their input
			char *buffer;
    			size_t bufsize = 32;
			buffer = (char *)malloc(bufsize * sizeof(char));
			getline(&buffer, &bufsize, stdin);
			printf("you typed %s\n",buffer);


			//parse each string in the command into seperate strings
			char* argv[100];
			int argc =1;
			int i =0;

			//seperate the input by spaces
			char delim[] = " ";
			char *token;

			/* get the first token */
			token = strtok(buffer, delim);

			/* walk through other tokens */
   			while( token != NULL ) {
				token[strcspn(token, "\n")] = 0;
				argv[i++] = token;
      				//printf( "token in loop:  %s\n", token );
				argc +=1;

      				token = strtok(NULL, delim);
			}//end reading string into array
			argv[i++] = NULL;//Set the first location in the array after your C-Strings to NULL.
			//printf("first thing in argv: %s\n", argv[0]);


			//START handle array funciton
		 	running = handleArray(argv);
		}//end if it is not a batch file

	}//end main program while loop
}//end main

int handleArray(char **argv)
{
	int running =1;



	printf("The command is: %s\n", argv[0]);


                if( strcmp(argv[0], "cd") ==0)
                {
                        char currentPath[100];
                        printf("you picked cd\n");
                        if(argv[1] == NULL)
                        {
                                getcwd(currentPath, sizeof(currentPath));
                                printf("current path: %s\n", currentPath);
                        }//end if they dont enter a dir to go to
                        else//if they enter a dir
                        {


                                char *newDir = argv[1];

                                if(chdir(newDir) !=0)
                                {
                                        printf("no directory exists\n");
                                }
                                else//if the directory exists
                                {
                                        printf("new directory: %s\n", newDir);
                                }


                                getcwd(currentPath, sizeof(currentPath));
                                printf("current path: %s\n", currentPath);
                        }//end if they enter a dir


                }//end if they enter CD
                else if(strcmp(argv[0], "clr") ==0)
                {
                        printf("you picked clr\n");
                        printf("\033[H\033[2J");
                }
                else if(strcmp(argv[0], "dir") ==0)
		{
			printf("you picked dir\n");

                        DIR *dir;
                        struct dirent *s;

                        char *directory = argv[1];
                        dir = opendir(directory);

			//if there is output redirection >, open a file
			//write to that file

			//if there is output redirection >>, open
			//append to that file

			//else if there is no redirection,
			//print normally

			int outPlace =-1;
			outPlace = findOutPlace(argv);//see where > is
			int redirectStatus= checkRedirect(argv);//check$
                        if(redirectStatus == 1)//>
                       	{
				FILE *fptr =fopen(argv[outPlace+1],"w");
				//printf("opened file %s\n",argv[outPlace+1]);

				//write to the file

				while((s = readdir(dir)) != NULL)
                        	//While there is another item in the directory that has not been looked at yet
                        	{
                                	fprintf(fptr, "%s\t", s->d_name);
                        	}

				fclose(fptr);
				//printf("there was output redirection > at place %d\n", outPlace);
			}//end if >
			else if(redirectStatus == 2)//>>
			{

				FILE *fptr =fopen(argv[outPlace+1],"a");
                                //printf("opened file %s\n",argv[outPlace+1]);

                                //write to the file

                                while((s = readdir(dir)) != NULL)
                                //While there is another item in the directory $
                                {
                                        fprintf(fptr, "%s\t", s->d_name);
                                }

                                fclose(fptr);
			}//end if >>
			else if( redirectStatus ==-1)//fi there is no redirection
			{
                        	//If dir equals NULL, that means there is an error
                        	while((s = readdir(dir)) != NULL)
                        	//While there is another item in the directory that has not been looked at yet
                        	{
                                	printf("%s\t", s->d_name);
                        	}
			}//end if there is no redirection

                }//end if they pick dir
                else if(strcmp(argv[0], "environ") ==0)
                {
                        printf("you picked environ\n");
                        printf("OSTYPE: %s\n", getenv("OSTYPE"));
                        printf("LANG: %s\n", getenv("LANG"));
                        printf("USER: %s\n", getenv("USER"));
                        printf("LOGNAME: %s\n", getenv("LOGNAME"));
                        printf("HOME: %s\n", getenv("HOME"));
                        printf("PATH: %s\n", getenv("PATH"));
                        printf("MAIL: %s\n", getenv("MAIL"));
                        printf("SHELL: %s\n", getenv("SHELL"));
                        printf("SSH_CLIENT: %s\n", getenv("SSH_CLIENT"));
                        printf("SSH_CONNECTION: %s\n", getenv("SSH_CONNECTION"));
			printf("SSH_TTY: %s\n", getenv("SSH_TTY"));
			printf("TERM: %s\n", getenv("TERM"));
			printf("HOSTTYPE: %s\n", getenv("HOSTTYPE"));
                        printf("WWW_HOME: %s\n", getenv("WWW_HOME"));

                }
                else if(strcmp(argv[0], "echo") ==0)
                {
                        printf("you picked echo\n");

                        //loop though argv until we hit null
                        int i =1;
                        while(argv[i] != NULL)
                        {
                                printf("%s ", argv[i]);
                                i+=1;
                        }//end while looping thorugh argv
                        printf("\n");
                }
                else if(strcmp(argv[0], "help") ==0)
                {
                        printf("you picked help\n");
                        //how the heck do i use the more command

			//exec more, pass it the help file
                }
                else if(strcmp(argv[0], "pause") ==0)
                {
                        printf("you picked pause\n");

                        //char *p;
                        //read -p "Press Enter to continue";
                        char c = getchar();
                        if(c == '\n')//if they hit the enter key
                        {
                                fflush(stdin);
                        }
                }
                else if(strcmp(argv[0], "exit") ==0)
                {
                        running =0;
                        printf("shell stopped\n");
			exit(0);
                }
                else//if it is an external command
                {



                        puts("this command is not built in");






                        int answer = fork();

                        if( answer ==0)//the child

			{

				int outPlace = -1;
				outPlace = findOutPlace(argv);

				int redirectStatus= checkRedirect(argv);//check if there is any redirection
				if(redirectStatus == 1)//>
				{

					int outFile = open(argv[outPlace+1], O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO); //get the file descriptor for the out file

                                        //replce stdout with outFIle
                                        close(1);
                                        dup2(outFile,1);
                                        close(outFile);

				}//end if >
				else if(redirectStatus == 2)//>>
				{

					int outFile = open(argv[outPlace+1], O_WRONLY|O_CREAT|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO); //get the file descriptor for the out file

                                        //replce stdout with outFIle
                                        close(1);
                                        dup2(outFile,1);
                                        close(outFile);

				}//end if >>



				//remove everything after >, or >> from the argv

				if(outPlace != -1)//if there is output redirection
				{
					//loop through the array after the > and change them to null
					int j = outPlace;
					while(argv[j] !=NULL)
					{
						argv[j] = NULL;
						j++;
					}//end looping through array of words
				}

                                if(execvp(argv[0], argv) < 0)
                                {
                                        puts("this is not a real command\n");
                                        exit(0);
                                }
                        }
                        else if(answer > 0)//the parent
                        {
                                //wait lines
                                //just the two lines
                                int status =0;
                                wait(&status);
                        }
                        else if(answer < 0)//error
                        {
                                puts("error");
                        }
                }//end if it is an external command


		return running;
}//end handle array funciton


//send it the array of words, it checks if there is output redirection of > or >>, returns 1 >, 2 for >>
int checkRedirect(char **argv)
{


                                //loop through argv to see if there is ">"
                                int i =0;
                                while(argv[i] != NULL)
                                {
                                        i+=1;
                                        if(strcmp(argv[i-1], ">") ==0)//if there is output redireciton
                                        {
                                                return 1;
                                        }//end if there is out redirection
                                        else if(strcmp(argv[i-1], ">>") ==0)//if there is append output redirection
                                        {
                                                return 2;

                                        }//end if there is append
                                }//end looping through argv
				return -1;



}//end check redirect funciton

//find the spot in the array that > or >> is located
int findOutPlace(char **argv)
{


				int outPlace = -1;
                                //loop through argv to see if there is ">"
                                int i =0;
                                while(argv[i] != NULL)
                                {
                                        i+=1;
                                        if(strcmp(argv[i-1], ">") ==0)//if there is output redireciton
                                        {
                                                outPlace = i-1;
                                                return outPlace;
                                        }//end if there is out redirection
                                        else if(strcmp(argv[i-1], ">>") ==0)//if there is append output redirection
                                        {
                                                outPlace = i-1;
                                                return outPlace;

                                        }//end if there is append
                                }//end looping through argv
	return outPlace;

}//end find outplace function

