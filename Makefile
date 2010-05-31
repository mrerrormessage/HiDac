CC=g++
PKLIBS=OGRE OIS
OGINCL= $(shell pkg-config --cflags $(PKLIBS))
CFLAGS=-Wall -g $(OGINCL)
JSONLD=-ljson -lstdc++
JSONHD=-I /usr/local/include
LIBS= $(shell pkg-config --libs $(PKLIBS)) $(JSONLD)
EXENAME=crowdsim


all: Agent.o CrowdObject.o Vector.o Wall.o CrowdWorld.o Render.o
	$(CC) $(CFLAGS) $(OGINCL) main.cpp *.o $(LIBS) -o $(EXENAME)

Agent.o: Agent.cpp
	$(CC) $(CFLAGS) -I. -c Agent.cpp

CrowdObject.o: CrowdObject.cpp
	$(CC) $(CFLAGS) -I. -c CrowdObject.cpp

CrowdWorld.o : CrowdWorld.cpp
	$(CC) $(CFLAGS) -I. -c CrowdWorld.cpp

Vector.o : vector.cpp
	$(CC) $(CFLAGS) -I. -c vector.cpp

Render.o : Render.cpp
	$(CC) $(CFLAGS) $(OGINCL) -I. -c Render.cpp

Wall.o : Wall.cpp
	$(CC) $(CFLAGS) -I. -c Wall.cpp

clean: 
	rm -f *.o *~ *.out $(EXENAME)