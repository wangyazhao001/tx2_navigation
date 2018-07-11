CC = arm-linux-gcc
LD = arm-linux-ld
EXEC = serial
OBJS = serial.o

CFLAGS +=
LDFLAGS += 

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS$(LDLIBS_$@))
	cp $(EXEC) /tftpboot

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

