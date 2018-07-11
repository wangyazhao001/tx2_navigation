#include "apue.h"
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <time.h>
#define BUFLEN		128

extern int connect_retry(int, int, int, const struct sockaddr *,
	socklen_t);

void
print_uptime(int sockfd,FILE *fd1)
{
	int		n;
	char	buf[BUFLEN];
	struct timespec	time_out;
	while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0)
	{
		clock_gettime(CLOCK_REALTIME, &time_out);
		fprintf(fd1,"%11ld%11ld",time_out.tv_sec,time_out.tv_nsec);
		fprintf(fd1, buf,n);		
	}
	if (n < 0)
		err_sys("recv error");
}

int
main(int argc, char *argv[])
{
	struct addrinfo	*ailist, *aip;
	struct addrinfo	hint;
	int		sockfd, err;
	FILE *fd1;
	int  res;
	fd1=fopen("/home/xc/pcxsens.txt","w");
	if(fd1 == NULL)
		perror("create file fail");

	if(res=setvbuf(fd1,NULL,_IONBF,0)!=0)
		printf("setvbuf failure\n");

	if (argc != 1)
		err_quit("usage: ruptime hostname  ; port");
	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((err = getaddrinfo("192.168.1.180", "2346", &hint, &ailist)) != 0)//xc
		err_quit("getaddrinfo error: %s", gai_strerror(err));
	for (aip = ailist; aip != NULL; aip = aip->ai_next) {
		if ((sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0,
		  aip->ai_addr, aip->ai_addrlen)) < 0) {
			err = errno;
		} else {
			print_uptime(sockfd,fd1);
			exit(0);
		}
	}
	err_exit(err, "can't connect to %s", argv[1]);
}
