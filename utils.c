#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/**
 * Reads a file by path and returns the relative allocated buffer
 * @param path path of the file
 * @return the buffer
 */

BUF* read_file(char* path){

	char *buf = NULL;

	FILE *fp = fopen(path, "rb");

	if (fp != NULL) {

    	if (fseek(fp, 0, SEEK_END) == 0) {

        	long bufsize = ftell(fp);

        	if (bufsize != -1) { 

        		buf = malloc(sizeof(char) * (bufsize + 1));

        		if (fseek(fp, 0, SEEK_SET) == 0) { 

        			int count = fread(buf, sizeof(char), bufsize, fp);

        			if ( count == bufsize ) {

						BUF *buffer = malloc(sizeof(BUF));

						buffer->buffer = buf;

						buffer->length = count;

						fclose(fp);

						return buffer;

        			}

				}

			}

    	}

    	fclose(fp);

	}

    printf("Error reading file %s\n", path);

	return NULL;

}

/**
 * Writes a file on a specific path
 * @param path path of the file
 * @param buffer the buffer to write
 * @param length lenght of the buffer
 * @return outcome
 */

int write_file(char* path,char* buffer,int length){

		FILE *fp = fopen(path,"wb");

		if (fp != NULL) {

			int count = fwrite(buffer,length,1,fp);
    	
        		if ( count ) {

					printf("The file '%s' was written\n", path);

					fflush(fp);

					fclose(fp);

					return 0;

        		}

			fclose(fp);

		}

		printf("Error writing file %s\n",path);

        return -1;

}

/**
 * Return the size of a specific file by path
 * @param path path of the file
 * @return size
 */

long get_file_size(char* path){

	FILE *fp = fopen(path, "rb");

	if (fp != NULL) {

    	if (fseek(fp, 0, SEEK_END) == 0) {
			
			int size = ftell(fp);
        	
			fclose(fp);
			
			return size;

		}

		fclose(fp);

	}
  	
	printf("Error getting file size of %s\n",path);

	return -1;

}

/**
 * Allocates memory and return a buffer
 * @param length number of elements
 * @param size size of a singel element
 * @return buffer
 */

char* get_buffer(int length,int size){

	unsigned char *buffer = malloc( length * size);

	if(buffer == NULL){

		printf("Can't alloc more memory, sorry :(\n");

        return NULL;

	}

	return buffer;

}