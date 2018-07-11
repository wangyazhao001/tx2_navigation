#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#define 

int
main(void)
{
	struct timespec	time_out;
	clock_gettime(CLOCK_REALTIME, &time_out);
	printf("tv_sec 	is :	%ld\n",time_out.tv_sec);
	printf("tv_nsec is :	%ld\n",time_out.tv_nsec);
		
	return 0;
}
