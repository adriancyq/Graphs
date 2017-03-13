# A simple makefile for CSE 100 PA4

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
    CPPFLAGS += -O3
    LDFLAGS += -O3
else
    CPPFLAGS += -g
    LDFLAGS += -g
endif

all: pathfinder actorconnections extension



# include what ever source code *.h files pathfinder relies on (these are merely the ones that were used in the solution)

pathfinder: ActorGraph.o
actorconnections: ActorGraph.o
extension: ATPGraph.o

# include what ever source code *.h files ActorGraph relies on (these are merely the ones that were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired *.h file that is already included with class/method headers

#ActorGraph.o: UnionFind.hpp Movie.h ActorNode.h ActorEdge.h ActorGraph.h
ActorGraph.o: Movie.h ActorNode.h ActorGraph.h
ATPGraph.o: Games.h ATPNode.h ATPGraph.h 

clean:
	rm -f pathfinder *.o core*
	rm -rf pathfinder.dSYM
	rm -f actorconnections
	rm -rf actorconnections.dSYM
	rm -f extension *.o core*
	rm -rf extension.dSYM
