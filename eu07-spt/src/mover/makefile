COMPILER=g++
CCFLAGS=-g -Wall -iquote../../include

LIBS=
OBJS=Mover.o

core: ${OBJS}
	ar cr ../mover.a ${OBJS}

${OBJS}: %.o: %.cpp
	${COMPILER} -c ${CCFLAGS} $< -o $@

clean:
	rm -f *.o ../mover.a 
