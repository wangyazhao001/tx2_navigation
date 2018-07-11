CXX=g++
SOURCES=main.cpp
SOURCES+=xiApiPlusOcv.cpp SocketServer.cpp #header.hpp
OBJECTS=$(SOURCES:.cpp=.o)
PROGRAM=camera


all: $(PROGRAM)

$(PROGRAM): xiApiPlusOcv.o main.o SocketServer.o
	g++ $(OBJECTS) -o $(PROGRAM) -lm3api -lopencv_core -lopencv_highgui -lopencv_imgproc -L/usr/local/lib 

xiApiPlusOcv.o: xiApiPlusOcv.cpp
	g++ -c xiApiPlusOcv.cpp -I . -I /usr/local/include/ -g3 -Wall -c -fmessage-length=0
SocketServer.o : SocketServer.cpp
	g++ -c SocketServer.cpp -I . -I /usr/local/include -g3 -Wall -c -fmessage-length=0
main.0: main.cpp
	g++ -c main.cpp -I .  -I /usr/local/include/ -g3 -Wall -c -fmessage-length=0

clean:
	rm -f *.o *~ $(PROGRAM) $(OBJECTS)
