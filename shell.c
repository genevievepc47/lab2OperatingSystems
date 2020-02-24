#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//gen cohen
//fiore operating systems 2020 spring
//lab 2
//making a shell
int main()
{

	int running =1;//set to 0 when want to stop
	//main program loop
	while(running ==1)
	{
		printf("myshell> ");//print the prompt

		//get their input
		char *buffer;
    		size_t bufsize = 32;
		buffer = (char *)malloc(bufsize * sizeof(char));
		getline(&buffer, &bufsize, stdin);
		printf("you typed %s\n",buffer);


		if(strcmp(buffer, "exit\n") ==0)//if they type exit
		{
			running =0;
			printf("shell stopped\n");
		}

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

			//If dir equals NULL, that means there is an error
			while((s = readdir(dir)) != NULL)
			//While there is another item in the directory that has not been looked at yet
			{
				printf("%s\t", s->d_name);
			}

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
			printf("USER: %s\n", getenv("USER"));
			printf("USER: %s\n", getenv("USER"));


                }
		else if(strcmp(argv[0], "echo") ==0)
                {
                        printf("you picked echo\n");
                }
		else if(strcmp(argv[0], "help") ==0)
                {
                        printf("you picked help\n");
                }
		else if(strcmp(argv[0], "pause") ==0)
                {
                        printf("you picked pause\n");
                }
		else if(strcmp(argv[0], "exit") ==0)
		{
			//do nothing, already handled
		}
		else//if it is an external command
		{
			printf("this command is not built in\n");

			int answer = fork();

			if( answer ==0)//the child
			{
				if(execvp(argv[0], argv) < 0)
				{
					//an error
				}
			}
			else if(answer > 0)//the parent
			{
				//wait lines
				//just the two lines
			}
			else if(answer < 0)//error
			{
			}
		}

	}//end main program while loop
}//end main
