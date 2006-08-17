COMPILER=g++
CCFLAGS=-g -Wall

LIBS=
OBJS=MovementPath.o PathFollower.o SkyDome.o Scenery.o 

core: ${OBJS}
	cd events && make
	ar cr ../core.a events.a ${OBJS}

clean:
	cd events && make clean
	rm -f *.o ../core.a 
