COMPILER=g++
CCFLAGS=-g -pedantic -Wall -iquote../../include

LIBS=
OBJS=MovementPath.o PathFollower.o SkyDome.o ObjectsList.o Scenery.o 

core: ${OBJS}
	ar cr ../core.a ${OBJS}

${OBJS}: %.o: %.cpp
	${COMPILER} -c ${CCFLAGS} $< -o $@

clean:
	rm -f *.o ../core.a 
