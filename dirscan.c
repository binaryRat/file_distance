#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "utils.h"
#include "fdistance.h"

int minimum_distance = -1;
char minimum_distance_dir[4096]; //4096 maximum path lenght

static int scan_all_directory(char*,char*,int);

static int validate_path(char*);

static void setGlobalVariables(int,char*);

static void print_result(int,int, char*);

/**
 * Checks if a file name is acceptable
 * @param name name of the file
 * @return 
 */

static int validate_path(char* name){

    if(!strcmp(name,".")) return 1;

    if(!strcmp(name,"..")) return 1;

	return 0;

}

/**
 * Updates global variables value
 * @param distance distance from target file
 * @param path path of the file
 * @return 
 */

static void setGlobalVariables(int distance,char* path){

    minimum_distance = distance;

    strncpy(minimum_distance_dir,path,4096);

	return;
	
}

/**
 * Checks the distance and the limit and prints the results
 * @param distance distance from target file
 * @param limit limit of the distance
 * @param path path of the file
 * @return 
 */

static void print_result(int distance,int limit, char* path){

	if(distance <= limit && limit != -1) printf("Distance : %d Path : '%s'\n",distance,path);

    if((distance < minimum_distance || minimum_distance == -1) && limit == -1) setGlobalVariables(distance,path);

	return;

}

/**
 * Main function that recursively scans every directory
 * @param directory target directory
 * @param input_file file to compare with
 * @param limit limit of the distance
 * @return outcome (0 good, -1 bad)
 */

static int scan_all_directory(char* directory,char* input_file,int limit){

	DIR *d = opendir(directory);

    if (d){

    	struct dirent *dir;

    	while ((dir = readdir(d)) != NULL){

			if(validate_path(dir->d_name)) continue;
    		
      		char file_path[strlen(directory) + strlen(dir->d_name)];
      		
       		strcpy(file_path,directory);

       		strcat(file_path,dir->d_name);

			if(get_file_size(file_path) != -1){

				print_result(get_distance(input_file,file_path),limit,file_path);

			}else{

				scan_all_directory(strcat(file_path,"/"),input_file,limit);

			}
			
    	}

    	closedir(d);



	}else{

		return -1;

	}

	return 0;

}

/**
 * Prints every file from a directory tree that has distance less than a limit
 * @param input_file file to compare with
 * @param directory target directory
 * @param limit limit of the distance
 * @return outcome (0 good, -1 bad)
 */

int scan_directory(char* input_file,char* directory,int limit){

	int outcome = scan_all_directory(directory,input_file,limit);

	return outcome;

}

/**
 * Finds the file with minimum distance from imput file
 * @param input_file file to compare with
 * @param directory target directory
 * @return outcome (0 good, -1 bad)
 */

int find_min_distance(char* input_file,char* directory){

	int outcome = scan_all_directory(directory,input_file,-1);

	printf("Distance : %d File : %s\n",minimum_distance, minimum_distance_dir);

	return outcome;

}