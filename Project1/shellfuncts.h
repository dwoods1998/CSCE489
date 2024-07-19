/*************************************************************************************
 * Author: 1st Lt Dalton Woods
 * shellfuncts.h - header file for my shell functions
 *************************************************************************************/
#ifndef SHELL_FUNCTS_H
#define SHELL_FUNCTS_H

int createNewFile(const char *fileName);
int updateFile(const char *fileName, int number, const char *text);
int listFileContents(const char *fileName);
void listDirectoryContents();
void userCommand(char *command);
void parseCommand(char *command, char **args, int *numArgs, _Bool *background);

#endif // SHELL_FUNCTS_H

