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

//HAVING A PROBLEM WITH OUTPUT REDIRECTION FOR HELP
//WHENEVER U DO HELP > THEN HELP>> ONLY ONE HELP SHOWS UP IN THE FILE


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
				fprintf(fptr, "\n");

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
				fprintf(fptr, "\n");

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
				printf("\n");
			}//end if there is no redirection

                }//end if they pick dir
                else if(strcmp(argv[0], "environ") ==0)
                {
                        printf("you picked environ\n");

			int outPlace =-1;
                        outPlace = findOutPlace(argv);//see where > is
                        int redirectStatus= checkRedirect(argv);//check$
                        if(redirectStatus == 1)//>
                        {
                                FILE *fptr =fopen(argv[outPlace+1],"w");
                                //printf("opened file %s\n",argv[outPlace+1]);

                                //write to the file

				fprintf(fptr, "OSTYPE: %s\n", getenv("OSTYPE"));
                        	fprintf(fptr, "LANG: %s\n", getenv("LANG"));
                        	fprintf(fptr, "USER: %s\n", getenv("USER"));
                        	fprintf(fptr, "LOGNAME: %s\n", getenv("LOGNAME"));
                        	fprintf(fptr, "HOME: %s\n", getenv("HOME"));
                        	fprintf(fptr, "PATH: %s\n", getenv("PATH"));
                        	fprintf(fptr, "MAIL: %s\n", getenv("MAIL"));
                        	fprintf(fptr, "SHELL: %s\n", getenv("SHELL"));
                        	fprintf(fptr, "SSH_CLIENT: %s\n", getenv("SSH_CLIENT"));
                        	fprintf(fptr, "SSH_CONNECTION: %s\n", getenv("SSH_CONNECTION"));
                        	fprintf(fptr, "SSH_TTY: %s\n", getenv("SSH_TTY"));
                        	fprintf(fptr, "TERM: %s\n", getenv("TERM"));
                        	fprintf(fptr, "HOSTTYPE: %s\n", getenv("HOSTTYPE"));
                        	fprintf(fptr, "WWW_HOME: %s\n", getenv("WWW_HOME"));
				fclose(fptr);
			}//end if >
			else if(redirectStatus == 2)//>>
                        {

                                FILE *fptr =fopen(argv[outPlace+1],"a");
                                //printf("opened file %s\n",argv[outPlace+1]);

                                //write to the file

				fprintf(fptr, "OSTYPE: %s\n", getenv("OSTYPE"));
                                fprintf(fptr, "LANG: %s\n", getenv("LANG"));
                                fprintf(fptr, "USER: %s\n", getenv("USER"));
                                fprintf(fptr, "LOGNAME: %s\n", getenv("LOGNAME"));
                                fprintf(fptr, "HOME: %s\n", getenv("HOME"));
                                fprintf(fptr, "PATH: %s\n", getenv("PATH"));
                                fprintf(fptr, "MAIL: %s\n", getenv("MAIL"));
                                fprintf(fptr, "SHELL: %s\n", getenv("SHELL"));
                                fprintf(fptr, "SSH_CLIENT: %s\n", getenv("SSH_CLIENT"));
                                fprintf(fptr, "SSH_CONNECTION: %s\n", getenv("SSH_CONNECTION"));
                                fprintf(fptr, "SSH_TTY: %s\n", getenv("SSH_TTY"));
                                fprintf(fptr, "TERM: %s\n", getenv("TERM"));
                                fprintf(fptr, "HOSTTYPE: %s\n", getenv("HOSTTYPE"));
                                fprintf(fptr, "WWW_HOME: %s\n", getenv("WWW_HOME"));
                                fclose(fptr);
                        }//end if >>
			else if( redirectStatus ==-1)//fi there is no redirection
                        {

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

			}//end if there is no redirection
                }
                else if(strcmp(argv[0], "echo") ==0)
                {
                        printf("you picked echo\n");

			int outPlace =-1;
                        outPlace = findOutPlace(argv);//see where > is
                        int redirectStatus= checkRedirect(argv);//check$
                        if(redirectStatus == 1)//>
                        {


				//have to remove the last two elements from argv

				FILE *fptr =fopen(argv[outPlace+1],"w");
                                //printf("opened file %s\n",argv[outPlace+1]);


				if(outPlace != -1)//if there is output redirect$
                                {
                                        //loop through the array after the > an$
                                        int j = outPlace;
                                        while(argv[j] !=NULL)
                                        {
                                                argv[j] = NULL;
                                                j++;
                                        }//end looping through array of words
                                }//end taking out after > or >>

                                //write to the file
				int i =1;
                        	while(argv[i] != NULL)
                        	{
                                	fprintf(fptr, "%s ", argv[i]);
                                	i+=1;
                        	}//end while looping thorugh argv
                        	fprintf(fptr,"\n");

				fclose(fptr);
			}//end if >
			else if(redirectStatus ==2)//>>
			{


				 FILE *fptr =fopen(argv[outPlace+1],"a");
                                //printf("opened file %s\n",argv[outPlace+1]);


				//remove everything after > or >>
                                if(outPlace != -1)//if there is output redirect$
                                {
                                        //loop through the array after the > an$
                                        int j = outPlace;
                                        while(argv[j] !=NULL)
                                        {
                                                argv[j] = NULL;
                                                j++;
                                        }//end looping through array of words
                                }//end removing words after > or >>

                                //write to the file

				int i =1;
                                while(argv[i] != NULL)
                                {
                                        fprintf(fptr, "%s ", argv[i]);
                                        i+=1;
                                }//end while looping thorugh argv
                                fprintf(fptr,"\n");
				fclose(fptr);
			}//end if >>
			else if(redirectStatus == -1)//if no redirect
			{

                        	//loop though argv until we hit null
                        	int i =1;
                        	while(argv[i] != NULL)
                        	{
                                	printf("%s ", argv[i]);
                                	i+=1;
                        	}//end while looping thorugh argv
                        	printf("\n");
			}//end if no redirect
                }//end if echo command
                else if(strcmp(argv[0], "help") ==0)
                {

			int outPlace =-1;
                        outPlace = findOutPlace(argv);//see where > is
                        int redirectStatus= checkRedirect(argv);//check$
                        if(redirectStatus == 1)//>
                        {
                                FILE *fptr =fopen(argv[outPlace+1],"w");
                                //printf("opened file %s\n",argv[outPlace+1]);

				printf("i am writing over\n");
                                //write to the file
                        	fprintf(fptr, "WELCOME TO HELP\n");
                        	fprintf(fptr, "BUILT IN COMMANDS:\n");
                        	fprintf(fptr, "cd: change the directory\n");
                        	fprintf(fptr, "clr: clear the screen\n");
                        	fprintf(fptr, "dir: print the contents of the current directory or user specified directory\n");
                        	fprintf(fptr, "environ: list the environment variables\n");
                        	fprintf(fptr, "echo: print the user's input\n");
                        	fprintf(fptr, "help: display the user manual\n");
                        	fprintf(fptr, "pause: pause the shell until the user presses enter\n");
                        	fprintf(fptr, "exit: exit the program\n");
                        	fprintf(fptr, "you can also type in any normal shell command\n");
                        	fprintf(fptr, "print your output to a file instead with >\n");
                        	fprintf(fptr, "append your output to a file with >>\n");
                        	fprintf(fptr, "give your input in a file with <\n");
                        	fprintf(fptr, "execute a command in the background with &\n");
                        	fprintf(fptr, "make the output of one command the input of another with |\n");
			}//end if >
			else if(redirectStatus ==2)//>>
			{
				FILE *fptr =fopen(argv[outPlace+1],"a");
                                //printf("opened file %s\n",argv[outPlace+1]);


				printf("I am appending\n");
                                //write to the file
                                fprintf(fptr, "WELCOME TO HELP\n");
                                fprintf(fptr, "BUILT IN COMMANDS:\n");
                                fprintf(fptr, "cd: change the directory\n");
                                fprintf(fptr, "clr: clear the screen\n");
                                fprintf(fptr, "dir: print the contents of the current directory or user specified directory\n");
                                fprintf(fptr, "environ: list the environment variables\n");
                                fprintf(fptr, "echo: print the user's input\n");
                                fprintf(fptr, "help: display the user manual\n");
                                fprintf(fptr, "pause: pause the shell until the user presses enter\n");
                                fprintf(fptr, "exit: exit the program\n");
                                fprintf(fptr, "you can also type in any normal shell command\n");
                                fprintf(fptr, "print your output to a file instead with >\n");
                                fprintf(fptr, "append your output to a file with >>\n");
                                fprintf(fptr, "give your input in a file with <\n");
                                fprintf(fptr, "execute a command in the background with &\n");
                                fprintf(fptr, "make the output of one command the input of another with |\n");

			}//end if >>
			else if(redirectStatus ==-1)//no redirection
			{

                        	printf("you picked help\n");
                        	printf("WELCOME TO HELP\n");
				printf("BUILT IN COMMANDS:\n");
				printf("cd: change the directory\n");
				printf("clr: clear the screen\n");
				printf("dir: print the contents of the current directory or user specified directory\n");
				printf("environ: list the environment variables\n");
				printf("echo: print the user's input\n");
				printf("help: display the user manual\n");
				printf("pause: pause the shell until the user presses enter\n");
				printf("exit: exit the program\n");
				printf("you can also type in any normal shell command\n");
				printf("print your output to a file instead with >\n");
				printf("append your output to a file with >>\n");
				printf("give your input in a file with <\n");
				printf("execute a command in the background with &\n");
				printf("make the output of one command the input of another with |\n");
			}//end if no redirection

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

