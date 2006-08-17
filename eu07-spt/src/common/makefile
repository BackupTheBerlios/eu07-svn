COMPILER=g++
CCFLAGS=-g -Wall

LIBS=
OBJS=Exception.o FindNodeVisitor.o 

fileio: ${OBJS}
	ar cr ../common.a ${OBJS} ${LIBS}

Exception.o: Exception.cpp
	${COMPILER} ${CCFLAGS} -c Exception.cpp

FindNodeVisitor.o: FindNodeVisitor.cpp
	${COMPILER} ${CCFLAGS} -c FindNodeVisitor.cpp

clean:
	rm -f *.o ../common.a 
