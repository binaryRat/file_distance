#ifndef DATA_STRUCT
	#define DATA_STRUCT
	typedef struct
	{
   		char type[3];
   		int pos;
    	char value;
	} Data;
#endif

int change_file(char*,char*,char*);