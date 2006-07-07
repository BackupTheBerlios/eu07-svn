COMPILER=g++
CCFLAGS=-g -Wall

LIBS=-losg -losgDB -losgProducer
OBJS=EventsTest.o Events.a TextEvent.o TextEventReceiver.o
EV_OBJS=Event.o EventReceiver.o EventQueue.o EventIds.o EventMgr.o

EventsTest: ${OBJS} 
	${COMPILER} ${CCFLAGS} ${LIBS} -o EventsTest ${OBJS}

EventsTest.o: EventsTest.cpp
	${COMPILER} ${CCFLAGS} -c EventsTest.cpp

Events.a: ${EV_OBJS}
	ar cr Events.a ${EV_OBJS}

EventMgr.o: EventMgr.cpp
	${COMPILER} ${CCFLAGS} -c EventMgr.cpp

Event.o: Event.cpp
	${COMPILER} ${CCFLAGS} -c Event.cpp

EventReceiver.o: EventReceiver.cpp
	${COMPILER} ${CCFLAGS} -c EventReceiver.cpp

EventIds.o: EventIds.cpp
	${COMPILER} ${CCFLAGS} -c EventIds.cpp

EventQueue.o: EventQueue.cpp
	${COMPILER} ${CCFLAGS} -c EventQueue.cpp

TextEvent.o: TextEvent.cpp
	${COMPILER} ${CCFLAGS} -c TextEvent.cpp

TextEventReceiver.o: TextEventReceiver.cpp
	${COMPILER} ${CCFLAGS} -c TextEventReceiver.cpp

clean:
	rm -f *.a *.o a.out
