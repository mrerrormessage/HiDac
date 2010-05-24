CC=g++
CFLAGS=-Wall
JSONLD=-ljson -lstdc++
JSONHD=-I /usr/local/include
LIBS= $(JSONLD)
EXENAME=crowdsim


all: Agent.o CrowdObject.o Vector.o
	$(CC) $(CFLAGS) main.cpp *.o $(LIBS) -o $(EXENAME)

Agent.o: Agent.cpp
	$(CC) $(CFLAGS) -I. -c Agent.cpp

CrowdObject.o: CrowdObject.cpp
	$(CC) $(CFLAGS) -I. -c CrowdObject.cpp

Vector.o : vector.cpp
	$(CC) $(CFLAGS) -I. -c vector.cpp

clean: 
	rm -f *.o *~ *.out $(EXENAME)