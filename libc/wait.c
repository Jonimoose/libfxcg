#include <sys/types.h>
#include <stdio.h>
pid_t wait(int *stat_loc){
	fputs("wait\n",stderr);
	return 0;
}

pid_t wait3(int *status, int options,struct rusage *rusage){
	fputs("wait3\n",stderr);
	return 0;
}
