/*************************************************************************************
 * Author: 1st Lt Dalton Woods
 * myshell - code for Project 1 of CSCE 489
 * 
 * My personal experience with the C programming language is very limited.  After
 * creating the base code for main method below I utilized ChatGPT for 
 * debugging and overall optimization.  I also used it extensively as a learning tool
 * to help me understand the specific syntax required by C.  It was also usefull in
 * recomending potential best-practices for error handling and how to effectively 
 * utilize different libraries depending on what I was trying to get the code to do.
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "shellfuncts.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


#define BufferSize 256


int main() {
	

	char command[BufferSize];
	printf("Welcome to my Shell. If you need assistance please use 'help'\n\n");
	
	while(1){
		printf("Shell Process ID:  %d\n", getpid());
		printf("Enter command: ");

		//This handles errors for end-of-file conditions
		if (fgets(command, sizeof(command), stdin)==NULL){
			if (feof(stdin)){
				break;
			}
			perror("There was an Error. Please re-enter your command.");
			continue;	
		}
		
		size_t len = strlen(command);
		if (len > 0 && command[len-1] == '\n'){
			command[len-1] = '\0';
		}

		if (strcmp(command, "halt") == 0){
			
			printf("Exiting the shell.... \n");
			
			sleep(5); //adds a delay for clearing the terminal
			system("clear"); //clears the command terminal
			break;

		}
		
		//calls my userCommand function to parse/execute the desired command
		userCommand(command);

		printf("\n"); //adds a blank line after every output
	}

	return 0;
}