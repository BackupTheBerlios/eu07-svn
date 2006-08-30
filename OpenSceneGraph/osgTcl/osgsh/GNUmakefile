TARGET = osgsh
OUTDIR = ../bin

OBJS=osgsh.o
LIBS=-losgTcl

OUTFILE = $(OUTDIR)/$(TARGET)

osgsh: ${OBJS}
	g++ ${LIBS} -o ${OUTFILE} ${OBJS}

$(OBJS): %.o: %.cpp
	g++ -Wall -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o
