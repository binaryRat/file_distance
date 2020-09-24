#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "fdistance.h"

static int minimum(int,int,int);

static int** create_matrix(int,int);

static int** fill_matrix(int**,char*,char*);

static void free_matrix(int**,int);

static void add_intruction(Data*,char*,int,char);

static Data* reverse_matrix(int**,char*,char*,int,int);

static int find_direction(int** ,int ,int );

/**
 * Create a matrix and returns a pointer
 * @param m heigth of the matrix
 * @param n length of the matrix
 * @return the matrix pointer
 */

static int** create_matrix(int m,int n){

    int **matrix = (int**) get_buffer((m + 1) , sizeof(int*));

    if(matrix == NULL) return NULL;

    for(int i = 0; i <= m; i++){
        matrix[i] = (int*) get_buffer((n + 1) , sizeof(int));

        if(matrix[i] == NULL) return NULL;

    }

    return matrix;

}

/**
 * Applies the Levenshtein algorithm to a specific matrix 
 * https://it.wikipedia.org/wiki/Distanza_di_Levenshtein
 * @param matrix target matrix
 * @param buffer_1 first buffer
 * @param buffer_2 second buffer
 * @return filled matrix
 */

static int** fill_matrix(int** matrix,char* buffer_1,char* buffer_2){

    int m = strlen(buffer_1);

    int n = strlen(buffer_2);

    for(int i = 0; i <= m; i++) matrix[i][0] = i;

    for(int j = 1; j <= n; j++) matrix[0][j] = j;

    for(int i = 1; i <= m; i++) {

        for(int j = 1; j <= n; j++) {

            if(buffer_1[i - 1] != buffer_2[j - 1]) {

                int k = minimum( matrix[i][j - 1], matrix[i - 1][j], matrix[i - 1][j - 1]);

                matrix[i][j] = k + 1;

            } else {

                matrix[i][j] = matrix[i - 1][j - 1];

            }

        }

    }

    return matrix;
    
}

/**
 * Returns the Levenshtein created from 2 files
 * @param file_1 first file
 * @param file_2 second file
 * @return completed matrix
 */

static int** get_matrix(char* file_1,char* file_2){

    int m = strlen(file_1);

    int n = strlen(file_2);

    int **matrix = create_matrix(m,n);

    if(matrix == NULL) return NULL;

    matrix = fill_matrix(matrix,file_1,file_2);

    return matrix;

}

/**
 * Adds a single instruction to a buffer
 * @param instruction target buffer pointer
 * @param type type of mod
 * @param position position of mod
 * @param value value to write at the position
 * @return
 */

static void add_intruction(Data* instruction,char* type,int position,char value){

    memcpy(instruction->type,type, 3);

    instruction->pos = position;

    instruction->value = value;

    return;

}

/**
 * Finds i which direction the Levenshtein should procede
 * @param matrix Levenshtein matrix
 * @param m length
 * @param n width
 * @return direction (1 Left, 2 Top, 3 Diagonal)
 */

static int find_direction(int** matrix,int m,int n){

    int direction = 0;

    int k = 0;

    if(m > 0 && n > 0){

        k = minimum(matrix[m][n - 1], matrix[m - 1][n], matrix[m - 1][n - 1]);

        if(k == matrix[m][n - 1]) direction = 1;
                        
        else if(k == matrix[m - 1][n]) direction = 2;
                    
        else if(k == matrix[m - 1][n - 1]) direction = 3; 
                
    }else{

        if(m == 0) direction = 1;

        else if(n == 0) direction = 2;

    } 

    return direction;

}

/**
 * Cincle the matrix and creates the instruction buffer
 * @param matrix Levenshtein matrix
 * @param buffer_1 First File
 * @param buffer_2 Second FIle
 * @param m length
 * @param n width
 * @return Instructions buffer
 */

static Data* reverse_matrix(int** matrix,char* buffer_1,char* buffer_2,int m, int n){

        int distance = matrix[m][n];

        Data *instruction = (Data*) get_buffer( distance , sizeof(Data));   

        if(instruction == NULL) return NULL;

        int point = distance -1;

        while(m > 0 || n > 0){

            if(buffer_1[m-1] != buffer_2[n-1]) {
                
                int direction = find_direction(matrix,m,n);
                		
                if(direction == 1){

                    add_intruction(&instruction[point],"ADD",m,buffer_2[n-1]);

                    m++;

                }

                if(direction == 2){

                    add_intruction(&instruction[point],"DEL",m-1,' ');

                    n++;

                }

                if(direction == 3) add_intruction(&instruction[point],"SET",m-1,buffer_2[n-1]);

                point--;

            }

            m--;

            n--;

        }    
        
        return instruction;

}

/**
 * Returns the minimum value of 3 numbers
 * @param a number 1
 * @param b number 2
 * @param c number 3
 * @return minimum
 */

static int minimum(int a, int b, int c){

    int min = a;

    if (b < min) min = b;

    if (c < min) min = c;

    return min;

}

/**
 * Releases the memory of a matrix pointer
 * @param matrix A matrix
 * @param length the length of a matrix
 * @return 
 */

static void free_matrix(int** matrix,int length){

    for(int i = 0; i <= length; i++)
        free(matrix[i]);

    free(matrix);

    return;

}

/**
 * Finds the file distance and writes the instruction
 * @param first First file
 * @param second Second file
 * @param outfile output file
 * @return file distance
 */

int compute_distance(char *first, char *second,char *outfile) {

    char *file_1 = read_file(first);
    if(file_1 == NULL) return -1;

    char *file_2 = read_file(second);
    if(file_2 == NULL) return -1;

    int m = strlen(file_1);

    int n = strlen(file_2);

    int** matrix = get_matrix(file_1,file_2);

    int distance = matrix[m][n];

    
    Data *instructions = reverse_matrix(matrix,file_1,file_2,m,n);
    if(instructions == NULL) return -1;

    int wrote = write_file(outfile,(char*)instructions,(distance * sizeof(Data)));
    if(wrote == -1) return -1;    

    free(instructions);

    free_matrix(matrix,m);

    free(file_1);

    free(file_2);

    return distance;
}

/**
 * Finds the file distance
 * @param first First file
 * @param second Second file
 * @return file distance
 */

int get_distance(char *first, char *second){
    
    char *file_1 = read_file(first);
    if(file_1 == NULL) return -1;

    char *file_2 = read_file(second);
    if(file_2 == NULL) return -1;

    int m = strlen(file_1);

    int n = strlen(file_2);

    int** matrix = get_matrix(file_1,file_2);

    int distance = matrix[m][n];

    free_matrix(matrix,m);

    free(file_1);

    free(file_2);

    return distance;
}