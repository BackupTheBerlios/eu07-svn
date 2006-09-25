COMPILER=g++
CCFLAGS=-g -Wall -iquote../../include

LIBS=
OBJS=Event.o EventReceiver.o EventQueue.o EventIds.o EventMgr.o 

core: ${OBJS}
	ar cr ../events.a ${OBJS}

${OBJS}: %.o: %.cpp
	${COMPILER} -c ${CCFLAGS} $< -o $@

clean:
	rm -f *.o ../events.a 
