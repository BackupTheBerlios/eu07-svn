COMPILER=g++
CCFLAGS=-g -Wall

LIBS=-losg
OBJS=DataInputStream.o DataOutputStream.o ../common.a 

fileio: ${OBJS}
	ar cr ../fileio.a ${OBJS} 

DataInputStream.o: DataInputStream.cpp
	${COMPILER} ${CCFLAGS} -c DataInputStream.cpp

DataOutputStream.o: DataOutputStream.cpp
	${COMPILER} ${CCFLAGS} -c DataOutputStream.cpp

clean:
	rm -f *.o ../fileio.a 
