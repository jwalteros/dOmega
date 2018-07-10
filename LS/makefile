CPP       = g++
CPPARGS   = -O3 -m64 -std=c++14 -Wall -Wextra -pedantic -pthread
SRCPATH		= ./src/
BINPATH		= ./bin/
INSTANCE	= -e ../../../Codes/Instances/txt/Other/cage15.graph.txt -m 8

all:
	$(CPP) $(CPPARGS) $(SRCPATH)Clique.cpp \
	$(SRCPATH)Graph.cpp \
	$(SRCPATH)VertexCover.cpp \
	$(SRCPATH)NemhauserTrotter.cpp \
	$(SRCPATH)Buss.cpp -o $(BINPATH)dOmega $(SRCPATH)main.cpp

clean:
	rm -rf $(BINPATH)*.o $(BINPATH)*.dSYM $(BINPATH)main

run:
	$(BINPATH)dOmega $(INSTANCE)
