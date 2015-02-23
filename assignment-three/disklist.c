//////////////////////////
// CSC 360	Fall 2014	//
// Assignment 3 Part 2	//
// Jaimee Blackwood		//
// V00755181			//
//////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

// http://stackoverflow.com/questions/523724/c-c-check-if-one-bit-is-set-in-i-e-int-variable 
#define CHECK_BIT(var, pos) !!((var) & (1 << (pos)))

static int current_seg_size;		// current size of segment
static void *current_seg;			// current segment pointer

int get_block_size(int block_size, FILE *input){
		current_seg_size = 2;								//only one stored as 2 bytes
		current_seg = malloc(current_seg_size);
		fread(current_seg, 1, current_seg_size, input);
		memcpy(&block_size, current_seg, current_seg_size);
		current_seg_size = 4;								//back to 4 bytes for other info
		return ntohs(block_size);
}
int get_info(int x, FILE *input){
		current_seg = malloc(current_seg_size);
		fread(current_seg, 1, current_seg_size, input);
		memcpy(&x, current_seg, current_seg_size);
		return ntohl(x);
}
int shorten(FILE *input){
	return fread(current_seg, 1, current_seg_size, input);	//this is only here to make for loop below smaller/cleaner
}
int print_entry() {
	// values required to print for each directory entry 
	char status;
	int file_size, year, i;
	int date[2];
	int time[3];
	char file_name[31];

	memcpy(&status, current_seg, 1); // status byte
	
	if (!CHECK_BIT(status, 0)){						
		current_seg +=64;	// if first bit is 0, entry is available
		return 0;			// skip past, return to while loop 
	}
		
	current_seg += 9; // skips to file size
	// starting block/num of blocks are skipped & not required
	memcpy(&file_size, current_seg, 4);
	file_size = ntohl(file_size);
	
	current_seg += 11; // skips to modify time
	// create time is skipped & not required	
	for (i = 0;i < 7;i++) {
		if(i<2){
			memcpy(&year, current_seg, 2);		//stores modify year
			year = ntohs(year);
			current_seg++;
			i++;
		}else if(i<4)
			memcpy(&date[i-2], current_seg, 1);	//stores modify date
		else
			memcpy(&time[i-4], current_seg, 1);	//stores modify time
		current_seg++;
	}	

	memcpy(file_name, current_seg, 31);			//stores file name		
	
	// print listing
	if (CHECK_BIT(status, 1))	//if bit 1 is 1, normal file
		printf("F");
	else						//if not, bit 2 must be one, directory
		printf("D");
		
	printf("%10d%30s %4d/%.2d/%.2d %.2d:%.2d:%.2d\n",file_size,file_name,year,date[0],date[1],time[0],time[1],time[3] );
	
	current_seg += 37;	//skips pointer past unused, to end of directory entry, before returning
	return 0;
}

///////////////
/// M A I N ///		
///////////////	
int main (int argc, const char * argv[]) 
{
	if (argc < 2) {
		printf("Error: incorrect usage. Intended use: \'./disklist *.img\'");
		return 0;
	}
	
	FILE *input = fopen(argv[1], "r");
	
	int block_size = 0, root_start = 0, root_count = 0;
	current_seg_size = 4;
	
	fseek(input, 8, SEEK_CUR);								//skips file system identifier
	
		//get values
		block_size = get_block_size(block_size, input);
		fseek(input, (current_seg_size*3), SEEK_CUR);		//skips block_count, fat_start, and fat_count
		root_start = get_info(root_start, input);
		root_count = get_info(root_count, input);
	
	rewind(input);
	//move to root directory
	fseek(input, root_start * block_size, SEEK_CUR);	
	//directory entry size is 64 bytes
	current_seg_size = 64;								
	current_seg = malloc(current_seg_size);
	
	// shorten(input) returns fread(current_seg, 1, current_seg_size, input)
	// this loop points current_seg to the start of a directory entry
	// then prints it's details by calling the print_entry function
	// then increments to the start of the next directory entry
	// the max # of entries is root_count 
	int i;	
	for(i = shorten(input); i <= root_count*block_size; i += shorten(input)) {
		print_entry();													
	}
	return 0;
}