//////////////////////////
// CSC 360	Fall 2014	//
// Assignment 3 Part 1	//
// Jaimee Blackwood		//
// V00755181			//
//////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

static int current_seg_size;		// current size of segment
static void *current_seg;			// current segment pointer

int get_block_size(int block_size, FILE *input){
		current_seg_size = 2;		
		current_seg = malloc(current_seg_size);
		fread(current_seg, 1, current_seg_size, input);
		memcpy(&block_size, current_seg, current_seg_size);
		current_seg_size = 4;
		return ntohs(block_size);
}

int get_info(int x, FILE *input){
		current_seg = malloc(current_seg_size);
		fread(current_seg, 1, current_seg_size, input);
		memcpy(&x, current_seg, current_seg_size);
		return ntohl(x);
}

///////////////
/// M A I N ///		
///////////////		
int main(int argc, char**argv)
{
	if (argc < 2) {
		printf("Error: incorrect usage. Intended use: \'./diskinfo *.img\'");
		return 0;
	}
	FILE *input = fopen(argv[1], "r");
	
	int block_size = 0, block_count = 0, fat_start = 0, fat_count = 0, root_start = 0, 
	root_count = 0, free_blocks = 0, reserved_blocks = 0, allocated_blocks = 0;
	
	fseek(input, 8, SEEK_CUR);	

		//get values
		block_size = get_block_size(block_size, input);
		block_count = get_info(block_count, input);
		fat_start = get_info(fat_start, input);
		fat_count = get_info(fat_count, input);
		root_start = get_info(root_start, input);
		root_count = get_info(root_count, input);
		
	//back to beginning 
	rewind(input);
	//move to start of FAT
	fseek(input, block_size * fat_start, SEEK_CUR);
	
	int hex, bytes;
	bytes = 0;
	//count number of allocated, reserved, and free blocks 
	for(bytes = 0; bytes != block_size*fat_count;)
	{	
		bytes += fread(current_seg, 1, 4, input);
		memcpy(&hex, current_seg, 4);
		hex = ntohl(hex);
		if(hex == 0x00000000){
				free_blocks++;
		}else if(hex == 0x00000001){
				reserved_blocks++;
		}else{
				allocated_blocks++;
		}
	}
	
	printf("Super block information:\nBlock Size: %d\nBlock Count: %d\nFAT starts: %d",
	 block_size, block_count, fat_start);
	printf("\nFAT blocks: %d\nRoot directory start: %d\nRoot directory blocks: %d\n",
	 fat_count, root_start, root_count);
	printf("\nFAT information:\nFree Blocks: %d\nReserved Blocks: %d\nAllocated Blocks: %d\n", 
	 free_blocks, reserved_blocks, allocated_blocks);
	return 0;
}