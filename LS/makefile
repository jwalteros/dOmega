CPP           = g++
CPPARGS       = -O3 -m64 -std=c++14 -Wall -Wextra -pedantic -pthread
SRCPATH	      = ./src/
BINPATH	      = ./bin/
DATPATH	      = ./dat/
NUMPROCESSORS = 8
INSTANCE      = $(DATPATH)Wiki-Vote.dat

all:
	[ -d $(BINPATH) ] || mkdir -p $(BINPATH)
	$(CPP) $(CPPARGS) $(SRCPATH)Clique.cpp \
	$(SRCPATH)Graph.cpp \
	$(SRCPATH)VertexCover.cpp \
	$(SRCPATH)NemhauserTrotter.cpp \
	$(SRCPATH)Buss.cpp -o $(BINPATH)dOmega $(SRCPATH)main.cpp

clean:
	rm -rf $(BINPATH)*.o $(BINPATH)*.dSYM $(BINPATH)dOmega

run:
	$(BINPATH)dOmega -e $(INSTANCE) -m $(NUMPROCESSORS)
