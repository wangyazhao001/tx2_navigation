CC=gcc
CFLAGS=-Wall -O -g
all: udp_client udp_server
PROGRAM=udp_client udp_server
SOURCES=clconn2.c client.c server.c initsrv2.c

all: $(PROGRAM) initsrv2.o clconn2.o

%: $(CFLAGS)  %.c -lapue

udp_client : client.c clconn2.o
	$(CC) $(CFLAGS) -o udp_client client.c clconn2.o -lapue
udp_server : server.c initsrv2.o
	$(CC) $(CFLAGS) -o udp_server server.c initsrv2.o  -lapue

clean: 
	rm -f *.o $(PROGRAM) 
