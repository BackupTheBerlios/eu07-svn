COMPILER=g++
CCFLAGS=-g -Wall -iquote../../include

LIBS=
OBJS=Bezier.o DataInputStream.o DataOutputStream.o MovementPath.o PathFollower.o SkyDome.o ObjectsList.o Scenery.o

core: ${OBJS}
	ar cr ../core.a ${OBJS}

${OBJS}: %.o: %.cpp
	${COMPILER} -c ${CCFLAGS} $< -o $@

clean:
	rm -f *.o ../core.a 
