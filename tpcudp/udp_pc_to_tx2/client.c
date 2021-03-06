#include "apue.h"
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <time.h>

#define BUFLEN		128
#define TIMEOUT		20

void
sigalrm(int signo)
{
}

void
print_uptime(int sockfd, struct addrinfo *aip,FILE *fd1)
{
	int		n;
	char	buf[BUFLEN];
	struct timespec	time_out;
	buf[0] = 0;
	if (sendto(sockfd, buf, 1, 0, aip->ai_addr, aip->ai_addrlen) < 0)
		err_sys("sendto error");
	alarm(TIMEOUT);
	while((n = recvfrom(sockfd, buf, BUFLEN, 0, NULL, NULL)) > 0)
	{
		clock_gettime(CLOCK_REALTIME, &time_out);
		alarm(0);
		//fprintf(fd1,"%11ld%11ld",time_out.tv_sec,time_out.tv_nsec);
		fprintf(fd1, buf,n);
		//write(STDOUT_FILENO, buf, n);
	}
	if(n<0)
	{
		if (errno != EINTR)
			alarm(0);
		err_sys("recv error");	
	}
}

int
main(int argc, char *argv[])
{
	struct addrinfo		*ailist, *aip;
	struct addrinfo		hint;
	int					sockfd, err;
	struct sigaction	sa;

	FILE *fd1;
	int  res;
	fd1=fopen("/home/xc/test.txt","w");
	if(fd1 == NULL)
		perror("create file fail");

	if(res=setvbuf(fd1,NULL,_IONBF,0)!=0)
		printf("setvbuf failure\n");

	if (argc != 1)
		err_quit("usage: ruptime hostname");
	sa.sa_handler = sigalrm;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGALRM, &sa, NULL) < 0)
		err_sys("sigaction error");
	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_DGRAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((err = getaddrinfo("192.168.1.190", "2347", &hint, &ailist)) != 0)
		err_quit("getaddrinfo error: %s", gai_strerror(err));

	for (aip = ailist; aip != NULL; aip = aip->ai_next) {
		if ((sockfd = socket(aip->ai_family, SOCK_DGRAM, 0)) < 0) {
			err = errno;
		} else {
			print_uptime(sockfd, aip,fd1);
			exit(0);
		}
	}

	fprintf(stderr, "can't contact %s: %s\n", argv[1], strerror(err));
	exit(1);
}
