#ifndef DATA_STRUCT
	#define DATA_STRUCT
	typedef struct
	{
	    char type[3];
	    int pos;
	    char value;
	} Data;
#endif
int compute_distance(char*,char*,char*);
int get_distance(char*,char*);