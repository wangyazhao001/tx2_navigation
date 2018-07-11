#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
pid_t status;

system("./serial_out1 | tee -a gps_data_deal.txt");

if(status == -1)
	perror("system function error");

wait(0);

}



