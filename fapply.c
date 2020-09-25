#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "fapply.h"

static int predict_file_size(int,int,Data*);

static int apply_instruction(Data,char*,int);

static char* get_changed_file(BUF*,Data*,char*);

/**
 * Predicts the size of the result file after the aplied changes
 * @param original_size size of the original file
 * @param changes_num number of changes
 * @param changes the changes
 * @return new index in case it was modified by the change
 */

static int predict_file_size(int original_size,int changes_num,Data* changes){

	int predict_size = original_size;

	for(int i = 0;i<changes_num;i++){

		if(!strcmp(changes[i].type,"ADD")) predict_size++;

		if(!strcmp(changes[i].type,"DEL")) predict_size--;

	} 

	return predict_size;

}

/**
 * Applies a single change to a buffer at specific index
 * @param change change to apply
 * @param buffer target buffer
 * @param index index of the buffer
 * @return new index in case it was modified by the change
 */

static int apply_instruction(Data change,char* buffer,int index){

	if(!strcmp(change.type,"ADD")){

		buffer[index] = change.value;

		index++;

	}

	if(!strcmp(change.type,"SET")){

		buffer[index] = change.value;

		index++;

	}  

	return index;

}

/**
 * Applies the changes to the file and writes the result buffer
 * @param origin_buffer buffer of the file to modify
 * @param changes buffer with all the changes
 * @param result_buffer buffer where to the new file
 * @return filled result_buffer
 */

static char* get_changed_file(BUF* file_struct,Data* changes,char* result_buffer){

	if(result_buffer == NULL) return NULL;

	int origin_size = file_struct->length;

	char* origin_buffer = file_struct->buffer;

	int index = 0;

	int j = 0;

	for(int i = 0;i<=origin_size;i++){

		int need_next_char = 1;

		while(changes[j].pos == i){

			index = apply_instruction(changes[j],result_buffer,index);

			if(!strcmp(changes[j].type,"SET") || !strcmp(changes[j].type,"DEL")) need_next_char = 0;

			j++;

		}
		
		if(need_next_char){

			result_buffer[index] = origin_buffer[i];

			index++;

		}
		
	}

	return result_buffer;

}

/**
 * Creates a changed file based on input and a changes file
 * @param origin_file file to modify
 * @param file_mod changes file
 * @param output_file path to write the result file
 * @return outcome (0 good, -1 bad)
 */

int change_file(char *origin_file, char *file_mod,char *output_file) {

	int outcome = -1;

	BUF *file_1 = read_file(origin_file);

	if(file_1 != NULL){

		Data *file_2 = (Data*)read_file(file_mod)->buffer;

		if(file_2 != NULL){

			long number_of_mods = get_file_size(file_mod) / sizeof(Data);
	
			int fsize = predict_file_size(file_1->length, number_of_mods,file_2);

			char *result_buffer = get_changed_file(file_1, file_2, get_buffer(fsize,sizeof(char)));

			if(result_buffer != NULL){

					int wrote = write_file(output_file,result_buffer,(fsize * sizeof(char)));
					
    				if(wrote != -1) outcome = 0;

					free(result_buffer);
			}

			free(file_2);

		}

		free(file_1->buffer);
		free(file_1);

	}

	return outcome;
}