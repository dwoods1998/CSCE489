/*************************************************************************************
 * Author: 1st Lt Dalton Woods
 * myshell - code for Project 1 of CSCE 489
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "shellfuncts.h"
#include <sys/types.h>
#include <sys/wait.h>


#define BufferSize 256


int main() {
	

	char command[BufferSize];
	
	while(1){
		printf("Shell Process ID:  %d\n", getpid());
		printf("Enter command: ");

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
			//adds a delay for clearing the terminal
			sleep(5);
			system("clear");
			break;

		}
		
		//execute command
		userCommand(command);

		printf("\n"); //adds a blank line after every output
	}

	return 0;
}


