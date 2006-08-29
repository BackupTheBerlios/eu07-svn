COMPILER=g++
CCFLAGS=-g -Wall

LIBS=-losg -losgDB -losgProducer core.a events.a common.a 
OBJS=Main.o

eu07-spt: ${OBJS}
	cd core && ${MAKE}
	cd common && ${MAKE}
	cd events && ${MAKE}
	${COMPILER} ${LIBS} ${OBJS} -o eu07-spt
	chmod 777 eu07-spt

clean:
	cd core && ${MAKE} clean
	cd common && ${MAKE} clean
	cd events && ${MAKE} clean
	rm eu07-spt
