#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "filereader.h"
#include "computedistance.h"

static int minimum(int,int,int);

static int** create_matrix(int,int);

static int** fill_matrix(int**,char*,char*);
static void free_matrix(int**,int);

static Data add_intruction(char*,int,char);

static Data* reverse_matrix(int**,char*,char*,int,int);

static int** create_matrix(int m,int n){

    int **matrix = (int**) malloc((m + 1) * sizeof(int*));

    if(matrix == NULL){
        printf("Can't alloc more memory, sorry :(\n");
        return NULL;
    }

    for(int i = 0; i <= m; i++){
        matrix[i] = (int*) malloc((n + 1) * sizeof(int));
        if(matrix[i] == NULL){
            printf("Can't alloc more memory, sorry :(\n");
            return NULL;
        }
    }

    return matrix;

}
static int** fill_matrix(int** matrix,char* buffer_1,char* buffer_2){

    int m = strlen(buffer_1);
    int n = strlen(buffer_2);

    for(int i = 0; i <= m; i++)
        matrix[i][0] = i;

    for(int j = 1; j <= n; j++)
        matrix[0][j] = j;

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
static int** get_matrix(char* file_1,char* file_2){
    int m = strlen(file_1);
    int n = strlen(file_2);

    int **matrix = create_matrix(m,n);

    if(matrix == NULL) return NULL;

    matrix = fill_matrix(matrix,file_1,file_2);
    return matrix;
}
static Data add_intruction(char* type,int position,char value){

    Data instruction;

    memcpy(instruction.type,type, 3);

    instruction.pos = position;

    instruction.value = value;

    return instruction;

}
static Data* reverse_matrix(int** matrix,char* buffer_1,char* buffer_2,int m, int n){

        int distance = matrix[m][n];

        Data *instruction;

        instruction = malloc( distance * sizeof(Data));    
        if(instruction == NULL){
            printf("Can't alloc more memory, sorry :(\n");
            return NULL;
        }

        int point = distance -1;

        while(m > 0 || n > 0){

            if(buffer_1[m-1] != buffer_2[n-1]) {

                int k = 0;
                if(m > 0 && n > 0){
                    k = minimum(
                        matrix[m][n - 1],
                        matrix[m - 1][n],
                        matrix[m - 1][n - 1]
                    );
                    if(k == matrix[m][n - 1]){
                        instruction[point] = add_intruction("ADD",m,buffer_2[n-1]);
                        n--;
                        } 
                    else if(k == matrix[m - 1][n]){
                        instruction[point] = add_intruction("DEL",m-1,' ');
                        m--;
                    } 
                    else if(k == matrix[m - 1][n - 1]){
                        instruction[point] = add_intruction("SET",m-1,buffer_2[n-1]);
                        m--;
                        n--;
                    }  
                }else{
                    if(m == 0){
                        instruction[point] = add_intruction("ADD",m,buffer_2[n-1]);
                        n--;
                    }else if(n == 0){
                        instruction[point] = add_intruction("DEL",m-1,' ');
                        m--;
                    }
                } 
                point--;
            }else{
                m--;
                n--;
            }
        }    
        return instruction;
}
static int minimum(int a, int b, int c){

    int min = a;

    if (b < min) min = b;
    if (c < min) min = c;

    return min;
}
static void free_matrix(int** matrix,int length){
    for(int i = 0; i <= length; i++)
        free(matrix[i]);
    free(matrix);
    return;
}
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