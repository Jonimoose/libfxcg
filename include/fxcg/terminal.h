//These should only be called once
extern int holdStrTERM;
#define FXCGinitTerminal Bdisp_EnableColor(1); \
	holdStrTERM=0; \
	termxfxcg=0; \
	termyfxcg=24; \
	termFGfxcg=0xFFFF; \
	termBGfxcg=0; \
	stdin->fileno=0; \
	stdout->fileno=1; \
	stderr->fileno=2; \
	memset((void *)0xA8000000,0,384*216*2); \
	Bdisp_PutDisp_DD();
//The reason this is a macro is so the array will last for the lifetime of the program
#define FXCGgetArgs \
	int argc=1; \
	char ** argv; \
	char ARGCn=0; \
	puts("This program takes arguments enter them now"); \
	char ArgsScratchMem[1024]; \
	inputStrTiny(ArgsScratchMem,1024,0); \
	printf("%s was entered\n",ArgsScratchMem); \
	if(ArgsScratchMem[0]){ \
		char * argsLoop=ArgsScratchMem; \
		while(1){ \
			printf("Argument at offset %d\n",argsLoop-ArgsScratchMem); \
			++argc; \
			while(((*argsLoop)!=' ')&&((*argsLoop)!=0)) \
				++argsLoop; \
			if(*argsLoop==0) \
				break; \
			++argsLoop; \
		} \
	} \
	printf("argc=%d\n",argc); \
	argv=alloca(argc*sizeof(void *)); \
	argv[0]=&ARGCn; \
	if((argc>1)&&(ArgsScratchMem[0]!=0)){ \
		int LoopAr=1; \
		char * argsLoop=ArgsScratchMem; \
		while(1){ \
			argv[LoopAr]=argsLoop; \
			++LoopAr; \
			while(((*argsLoop)!=' ')&&((*argsLoop)!=0)) \
				++argsLoop; \
			if(*argsLoop==0) \
				break; \
			++argsLoop; \
		} \
		for(LoopAr=1;LoopAr<argc;++LoopAr) \
			printf("Found %s\n",argv[LoopAr]); \
	}
