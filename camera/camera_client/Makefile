CXX=g++
SOURCES=main.cpp
SOURCES+=SocketClient.cpp SocketClient.hpp
OBJECTS=SocketClient.o main.o
PROGRAM=client

all:$(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) $(OBJECTS)  -o $(PROGRAM) -lm3api -lopencv_core -lopencv_highgui -lopencv_imgproc -L /usr/local/lib 
SocketClient.o : SocketClient.cpp SocketClient.hpp
	$(CXX) -c SocketClient.cpp  -o SocketClient.o
main.o : main.cpp SocketClient.o SocketClient.hpp
	$(CXX) -c main.cpp	 SocketClient.o     -o main.o

clean:
	rm -f *.o *~ $(PROGRAM) $(OBJECTS)
