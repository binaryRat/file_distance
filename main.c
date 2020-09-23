#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "computedistance.h"
#include "changefile.h"
#include "directoryscan.h"

void printhelp(char*);

void printhelp(char* name){

	printf("Bad arguments,\n");
	printf("Show edit distance : %s distance file1 file2\n",name);
	printf("Create chages file : %s distance file1 file2 output\n",name);
	printf("Apply chages file : %s apply inputfile filem output\n",name);
	printf("Directory search : %s search inputfile dir\n",name);
	printf("Directory search by limit : %s searchall inputfile dir limit\n",name);
	return;
}

int main(int argc,char* argv[]){

	if(argc < 2){

		printhelp(argv[0]);
		return -1;

	}

	if(!strcmp(argv[1],"distance") && argc >= 4){

		char *output = NULL;
		output = argv[4];

		double time_spent = 0.0;
		clock_t begin = clock();

		int distance = -1;
		if(argc == 4) distance = get_distance(argv[2],argv[3]);
		if(argc == 5) distance = compute_distance(argv[2],argv[3],output);

		clock_t end = clock();
		time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

		if(distance >= 0){
			printf("EDIT DISTANCE: %d instructions\n",distance );
			printf("TIME: %f seconds\n",time_spent);
		}else{
			printf("Distance : something went wront\n");
		}

		return 0;

	}

	if(!strcmp(argv[1],"apply") && argc >= 5){

		int outcome = change_file(argv[2],argv[3],argv[4]);
		
		if(outcome) printf("Something went wrong\n");
		
		return 0;

	}

	if(!strcmp(argv[1],"search") && argc >= 4){

		int outcome = find_min_distance(argv[2],argv[3]);

		if(outcome == -1) printf("Something went wrong\n");
		
		return 0;

	}

	if(!strcmp(argv[1],"searchall") && argc >= 5){

		int outcome = scan_directory(argv[2],argv[3],atoi(argv[4]));

		if(outcome == -1) printf("Something went wrong\n");

		return 0;

	}

	printhelp(argv[0]);
	
	return -1;

}