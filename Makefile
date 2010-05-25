CC=g++
CFLAGS=-Wall
JSONLD=-ljson -lstdc++
JSONHD=-I /usr/local/include
LIBS= $(JSONLD)
EXENAME=crowdsim


all: Agent.o CrowdObject.o Vector.o Wall.o
	$(CC) $(CFLAGS) main.cpp *.o $(LIBS) -o $(EXENAME)

Agent.o: Agent.cpp
	$(CC) $(CFLAGS) -I. -c Agent.cpp

CrowdObject.o: CrowdObject.cpp
	$(CC) $(CFLAGS) -I. -c CrowdObject.cpp

Vector.o : vector.cpp
	$(CC) $(CFLAGS) -I. -c vector.cpp

Wall.o : Wall.cpp
	$(CC) $(CFLAGS) -I. -c Wall.cpp

clean: 
	rm -f *.o *~ *.out $(EXENAME)