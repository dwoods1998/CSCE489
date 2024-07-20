# AFIT-CSCE489-PROJ1


# Overview
The files contained in this project create a unix like shell.  The code will repeatedly prompt the user for several specific commands and will display error messages should the user erroniously enter an invalid command.

# myshell.c
myshell.c contains the main method for implementing the shell.  The shell interface operates under an infinite while loop unless the "halt" command is given.  If the user inputs one of the several commands the main will call a userCommand function defined in shellfuncts.c to determine what should happen next.  The main and the functions used could be contained within one file but i decided to break them out logically into two files for ease of use and to seperate out what each file does.  After each command enterred the shell will loop through and re-prompt the user for the next command creating a repetitive terminal.

# shellfuncts.c
shellfuncts.c contains the bulk of the code for my shell.  it contains all of the functions used within the shell and is called by the main in myshell.c.  When the user inputs a command, the main of the shell calls the userCommand function where the enterred information is parsed and calls sub-functions depending on what the command is.

# shellfuncts.h
shellfuncts.h contains the necessary headers for the functions used by the shell.

## User Commands
There are several commands available to the user:

-halt:  ends the shell by breaking the while loop used by the main method
-create: requires the user to enter a filename to create a new file. It checks if a file by that name already exists and if it does it displays an error to the user.
-dir: lists the files within the current directory
-list:  lists the contents of a file specified by the user. If that file does not exist it notifies the user. if the file is empty, nothing is displayed. 
-update: appends a specified file X number of times with specific text specified by the user.  It will not write to a file that does not exist and will warn the user if a file does not exist

--Users can designate a background command by putting an & at then of a command input.  This command will operate in the background and the shell will proceed to the next command input.  The output for background commands is muted.

--Unknown commands entered by the user will not be executed. The shell will notify the user that an unknown command was entered and will prompt the user for a new command. 



