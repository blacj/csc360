// Jaimee Blackwood
// V00755181
// CSC 360 Assignment 1
// RSI: Realistic Shell Interpreter
// September 2014 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

////////////COUNT////////////
//
// 	counts the number of arguments in command line
// 	takes in char array of input, returns int
// 
/////////////////////////////
int count(char* argies)
{	
	int i;
	int c;
	c = 0;
	for(i=0; argies[i]!='\0'; i++)
	{
		if(argies[i] == ' ')
		{
			c++;
		}
	}
	return (c+1);
}

//////////[COMMAND]//////////
//
// 	passed in: input from command line as char array
// 	runs linux commands using execv
//
/////////////////////////////
void command(char** array)
{
	int x;			// hopeful
	char* program_name; 
	char* bin = "/bin/";		
	char* usrbin = "/usr/bin/";	
			
	//for commands that have path /bin/<command>
	program_name = malloc(sizeof(char)*(strlen(array[0]+strlen(bin))));
	program_name = strcat(strcpy(program_name, bin),array[0]);
			
	x = execv(program_name, array);
	
	//for commands that have path /usr/bin/<command>
	program_name = malloc(sizeof(char)*(strlen(array[0]+strlen(usrbin))));
    program_name = strcat(strcpy(program_name, usrbin),array[0]);
			
	x = execv(program_name, array);
		
	exit(0);
	
}

////////////MAIN//////////////
int main()
{
	char input[256];			// for keyboard input
	char curwd[128];
	char origwd[128];
	getcwd(curwd, 128);
	getcwd(origwd, 128);
	int counter;				// used for array assigning
	
	while(1)
	// runs forever unless explicitly told to exit
	// user does so by inputting 'exit'
	{
		char* tokens;
		counter = 0;
		
		printf("RSI: %s > ", curwd);			//prints RSI + current directory
		fgets(input, sizeof(input), stdin);	//assigns keyboard input to input array
		input[strlen(input)-1] = '\0';		//removes end of line char
		
		// closes RSI if exit typed
		if( strcmp(input, "exit") == 0 )
		{
			return 0;
		}
		
		int arg_num = count(input);		//count args
		char** array = malloc((sizeof(char*)*(arg_num+1)));	//allocate mem
		tokens = strtok(input," ");		//tokenize input
		while(tokens != NULL)
		{
			// allocate mem for each word in double char array
			array[counter]=(char*)malloc((sizeof(char)*strlen(tokens)));
            // move from tokens to array
			strcpy(array[counter], tokens);
			
            tokens = strtok (NULL," "); //empty tokens for next command
			counter++;					
		}
		
		char* cd = "cd";
		if(strcmp(cd, array[0])==0)
		{	
			
			char* squiggle = "~";
			if(counter == 1 || strcmp(squiggle, array[1])==0)
			{
				chdir(origwd);
			}
			chdir(array[1]);
			getcwd(curwd, 128);
		}
			
		int pid = fork();
		//if pid != 0, then it's the parent
		if(pid!=0)
		{
			wait(NULL);
		}
		else
		{
			command(array); //run command!
		}
	}
	
	return 0;
}			