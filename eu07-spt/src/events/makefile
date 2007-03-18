COMPILER=g++
CCFLAGS=-g -Wall -iquote../../include

LIBS=
OBJS=Event.o EventMgr.o EventIds.o EventQueue.o EventReceiver.o

core: ${OBJS}
	ar cr ../events.a ${OBJS}

${OBJS}: %.o: %.cpp
	${COMPILER} -c ${CCFLAGS} $< -o $@

clean:
	rm -f *.o ../events.a 
