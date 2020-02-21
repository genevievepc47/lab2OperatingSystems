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


		if( access( argv[0], F_OK )!= -1 ) {
    			// file exists
			printf("this command is built in\n");
		}//end if the file exists
		else
		{
			printf("this command is not built in\n");
		}

	}//end main program while loop
}//end main
