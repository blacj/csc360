// Jaimee Blackwood
// V00755181
// CSC 360 Fall 2014
// Assignment 2
// Multi-Threaded Scheduler

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 25

struct train_thread
{
	int train_num;
	char direction; //will be stored as single lower case letter e or w
	int priority; // 1 high priority, 0 low priority
	int load_time;
	int cross_time;
	pthread_t thread;
};

/* queue for trains, there will be one for each station */
struct thread_queue
{
	struct train *head;
	struct train *next;
	int count;
};

typedef struct train_thread train;
typedef struct thread_queue queue;

static queue east = {NULL, NULL, 0};
static queue west = {NULL, NULL, 0};
static train *current;
static const char *input;

///////////////////
// E N Q U E U E //
///////////////////
void enqueue()
{
 //called from run_train
}

///////////////////////
// R U N _ T R A I N // 
///////////////////////
void *run_train(void *thread)
{
	train *this = (train*)thread;
	
	// suspend thread execution until train's loading time, then add to queue.
	usleep(this->load_time * 100000);	// 1 second == 1,000,000 microseconds.
	enqueue(this);
	
	char *dir = (char*)malloc(sizeof(char)*4);
	// I didn't store directions as strings and regretted it at this point
	strcpy(dir, "East");
	if(this->direction == 'e')
		strcpy(dir, "East");
	if(this->direction == 'w')
		strcpy(dir, "West");
	
}

//////////////////
// R E A D  I N //
//////////////////
int readinput(int num_trains)
{
	FILE * inputfile = fopen(input, "r");
//	char * line = (char*)malloc(sizeof(char)*LINE_LENGTH);
	int train_counter = 0;
	
	char line [LINE_LENGTH];
	printf("I make it here 1");
	while( train_counter < num_trains )
	{
		fgets(line, sizeof line, inputfile);
		//current.train_num = train_counter;
		switch(line[0])
		{
			case 'E':
			//	current->direction = 'e';
				//current->priority = 1;
				break;
			case 'e':
			//	current->direction = 'e';
			//	current->priority = 0;
				break;
			case 'W':
			//	current->direction = 'w';
			//	current->priority = 1;
				break;
			case 'w':
			//	current->direction = 'w';
			//	current->priority = 0;
				break;
		} 
		
		current->load_time = atoi(strtok((line+2), ","));
		current->cross_time = atoi(strtok(NULL, "\0")); 
		
		train_counter++;
		pthread_create(&current->thread, NULL, run_train, (void *)current);
		
	} 
	
	return 0;
}

/////////////
// M A I N //
/////////////
void main(int argc, char*argv[])
{
		/* verifies arguments are as expected */
		input = (char*)malloc(sizeof(char)*LINE_LENGTH);
		input = argv[1];
		int total_num_trains = atoi(argv[2]); 
		if(input == NULL || total_num_trains == 0)
		{
			printf("Error: Arguments");
		}
			printf("I make it here PLS");
		readinput(total_num_trains);
	
		
}