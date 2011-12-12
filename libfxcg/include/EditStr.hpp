////////// TEditStr //////////
typedef struct{
	int xpos;
	int ypos;
	int height;
	unsigned char*buffer;
} TEditStr;

int EditStr_Execute( unsigned char*hint, TEditStr*ed );
////////// TEditStr //////////