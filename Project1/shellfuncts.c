/*************************************************************************************
 * Author: 1st Lt Dalton Woods
 * shellfuncts.c - code definitions for myshell.c
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "shellfuncts.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define BufferSize 256
#define MaxArgs 10

/*************************************************************************************
 * createNewFile - Creates a new file by the desired filename, but only if a file by that 
 * 		name does not already exist
 *
 *		Params:	fileName - a string of characters
 *
 *************************************************************************************/

int createNewFile(const char *fileName) {
	//check if file exists
	if(access(fileName, F_OK) == 0){
		fprintf(stderr, "Error creating file '%s' : File already exists.\n", fileName);
		return -1;
	}

	//create a new file if there is not one with the same name already
	FILE *file =fopen(fileName, "w");

	//if there is an error or a file already by that name print an error message to user
	if (file == NULL){
		
		fprintf(stderr, "\n Error creating file '%s': %s\n", fileName, strerror(errno));
		return -1;
	} 

	//prints a message if completed successfully with the process ID
	printf("\n File Created. Child Process ID: %d\n", getpid());
	
	//closes the new file
	fclose(file);

	return 0;
}

/*************************************************************************************
 * updateFile - Append lines of text to the named file. In particular, the text string
 * 		<text> is appended to the end of the file <number> times and simulates a slow 
 * 		device by forcing it to sleep for a few seconds after each line
 *
 *    Params:   fileName - string of characters for the filename
 *				number - a positive integer number of times to append file
 *				text - the desired text string to append				
 *
 *************************************************************************************/

int updateFile(const char *fileName, int number, const char *text) {
	//check if file exists
	if(access(fileName, F_OK) != 0){
		fprintf(stderr, "\n Error updating '%s' : File does not exist.\n", fileName);
		return -1;
	}
	
	//open the file that is desired
	FILE *file = fopen(fileName, "a");
	//if file by the name entered does not exist then tell the user
	if (file == NULL) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	// uses loop to append text <number> times
	for (int i = 0; i < number; ++i){
		fprintf(file, "%s\n", text); //appends
		fflush(file); //writes to the disk
		//Force the os to sleep for a few seconds between each line
		sleep(strlen(text) / 5);
	}

	
	

	//Close the file
	fclose(file);

	//prints a message if completed successfully with the process ID
	printf("\n Update completed. Child process ID: %d\n", getpid());
	return 0; 

}

/*************************************************************************************
 * listFileContents - Displays the contents of the named file on the screen. If the file
 * 		does not exist, prints an error message and ends command
 *
 *    Params:   fileName - string of characters for the filename		
 *
 *************************************************************************************/
int listFileContents(const char *fileName){
	printf("\nChild Process ID: %d\n", getpid());
	//checks if file exists
	if(access(fileName, F_OK) != 0){
		fprintf(stderr, "\n'%s' does not exist.\n", fileName);
		return -1;

	}else {

		char command[BufferSize];
		//create and execute cat command
		snprintf(command, BufferSize, "cat %s", fileName);
		execl("/bin/sh", "sh", "-c", command, (char *)NULL);
		perror("Error executing cat command");
		exit(EXIT_FAILURE);
	}
	printf("\n");

}

/*************************************************************************************
 *  listDirectoryContents- Lists the content within a designated file
 *
 *    Params: fileName - plaintext filename of what file is desired to be read  			
 *
 *************************************************************************************/
void listDirectoryContents(){
	//Execute ls to list files in the current directory
	printf("  Child Process ID: %d\n\n", getpid());
	execlp("/bin/ls", "ls", (char *)NULL);

	//return an error if it fails
	perror("Error executing ls command");
	exit(EXIT_FAILURE);

	printf("\n");
}
/*************************************************************************************
 *  parseCommand- this function serves as a command parser.  It will break up commands
 * 		and split them as necessary. It also checks for background designation
 *
 *    Params: command - the command the user called
 * 			  args -  the args passed by other functions necessary to execute
 * 			  numArgs - count of the args that are passed for the command
 * 			  background - designator for if the process should run in the background		
 *
 *************************************************************************************/
void parseCommand(char *command, char **args, int *numArgs, bool *background){
	bool quotedText = false;
	char *start = command;
	char *end = command;

	*numArgs = 0;
	*background = false;
	
	while(*end != '\0'){
		if (*end == '\"'){
			quotedText = !quotedText;//this toggles the quote status
			
		}else if (*end == ' ' && !quotedText){
			*end = '\0'; //end of argument
			if (start != end){
				args[(*numArgs)++] = start;
			}
			start = end + 1;
			
		}
		end++;
	}

	//handle last argument
	if (start != end){
		args[(*numArgs)++] = start;
	}

	//check if a background command
	if(*numArgs > 0 && strcmp(args[*numArgs - 1], "&") == 0){
		*background = true;
		args[--(*numArgs)] = NULL; //remove & from arguments
	}
}


/*************************************************************************************
 *  userCommand- a function to accept user input and call the necessary functions 
 * 		depending on what the user wants to do.
 *
 *    Params: command - the user input command arguments  			
 *
 *************************************************************************************/
void userCommand(char *command){
	char *args[MaxArgs];
	int numArgs;
	bool background;

	//this parses the user commands
	parseCommand(command, args, &numArgs, &background);

	//this handles empty commands
	if (numArgs == 0){
		return;
	}

	pid_t pid = fork();

		if (pid <0){
			perror("Fork Failed");
			exit(EXIT_FAILURE);
		}

	if (pid == 0){
		//child process
		
		if (background){
			//redirects stdin to /dev/null if running in background
			//mutes the background command output
			freopen("/dev/null", "r", stdin);
			freopen("/dev/null", "w", stdout);
			freopen("/dev/null", "w", stderr);
		}
	
		if (strcmp(args[0], "create") == 0){
			//Checks create arguments
			if (numArgs != 2){
				fprintf(stderr, "You entered an invalid number of arguments for create. \n Please use: create FileName ");
				exit(EXIT_FAILURE);
			} 
			//calls creatNewFile
			createNewFile(args[1]);
				
		}else if (strcmp(args[0], "update") == 0){
			//Checks update arguments
			if(numArgs < 4){
				fprintf(stderr, "You entered an invalid number of arguments for update. \n Please use: update FileName NumberOfRows YourTextinQuotes ");
				exit(EXIT_FAILURE);
			}

				
			int number = atoi(args[2]);
			//concatenates all remaining arguments into text string
			char text[BufferSize] = {0};
			for (int i = 3; i < numArgs; ++i){
				if (i > 3){
					strncat(text, " ", sizeof(text) - strlen(text) - 1);
				}
				strncat(text, args[i], sizeof(text) - strlen(text)- 1);
			}
			//calls updateFile
			updateFile(args[1], number, text);

		}else if (strcmp(args[0], "list") == 0){
			//checks list arguments
			if(numArgs != 2){
				fprintf(stderr, "You entered an invalid number of arguments for list. \n Please use: list FileName");
				exit(EXIT_FAILURE);
			}
			//calls listFileContents
			listFileContents(args[1]);

		}else if (strcmp(args[0], "dir") == 0){
			//checks dir arguments
			if (numArgs != 1){
				fprintf(stderr, "You entered an invalid number of arguments for dir. \n Please use: dir ");
				exit(EXIT_FAILURE);
			}
			//calls listDirectoryContents
			listDirectoryContents();

		}else {
			//if user inputs something other than expected, let them know.
			fprintf(stderr, "You enterred an unknown command: %s\n Please try again.\n", args[0]);
			exit(EXIT_FAILURE);
		}

		if (background) {
			exit(EXIT_SUCCESS);
		}else {
			exit(EXIT_SUCCESS);
		}

	}else{
		if (!background){
			int status;
			waitpid(pid, &status, 0); //wait for child to complete if not background
		
		}else {
			fflush(stdout);
		}
	}
}


