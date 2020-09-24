#ifndef DATA_STRUCT
	#define DATA_STRUCT
	typedef struct Data
	{
	    char type[3];
	    int pos;
	    char value;
	} Data;
#endif

char* read_file(char*);

int write_file(char*,char*,int);

long get_file_size(char*);

char* get_buffer(int,int);
