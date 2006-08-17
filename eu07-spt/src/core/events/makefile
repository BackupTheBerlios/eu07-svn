COMPILER=g++
CCFLAGS=-g -Wall

EV_OBJS=Event.o EventReceiver.o EventQueue.o EventIds.o EventMgr.o

Events: ${EV_OBJS}
	ar cr ../events.a ${EV_OBJS}

clean:
	rm -f *.o ../events.a 
