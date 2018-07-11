CC=gcc
CFLAGS=-Wall -O -g
all: tcp_client tcp_server
PROGRAM=tcp_client tcp_server
SOURCES=clconn2.c client.c server.c initsrv2.c

all: $(PROGRAM) initsrv2.o clconn2.o

%: $(CFLAGS)  %.c -lapue

tcp_client : client.c clconn2.o
	$(CC) $(CFLAGS) -o tcp_client client.c clconn2.o -lapue
tcp_server : server.c initsrv2.o
	$(CC) $(CFLAGS) -o tcp_server server.c initsrv2.o  -lapue

clean: 
	rm -f *.o $(PROGRAM) 
