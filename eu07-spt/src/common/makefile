COMPILER=g++
CCFLAGS=-g -Wall -iquote../../include

LIBS=
OBJS=Exception.o FindNodeVisitor.o DJBHash.o 

core: ${OBJS}
	ar cr ../common.a ${OBJS}

${OBJS}: %.o: %.cpp
	${COMPILER} -c ${CCFLAGS} $< -o $@

clean:
	rm -f *.o ../common.a 
