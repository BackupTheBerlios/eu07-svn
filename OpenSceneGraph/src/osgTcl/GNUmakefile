TARGET = osgTcl
OUTDIR = ../lib

SRC_FILES = \
	entry.cpp \
	Interpreter.cpp \
	Command.cpp

OBJ_FILES = $(SRC_FILES:.cpp=.o)
LIBS += -losg -losgDB -losgIntrospection -ltcl

OUTFILE = $(OUTDIR)/lib$(TARGET).so

$(TARGET): $(OUTFILE)

$(OUTFILE): $(OBJ_FILES)
	g++ --shared $(LIBS) $(LDFLAGS) -o $@ $(OBJ_FILES)

.cpp.o:
	g++ -Wall $(CFLAGS) -c $<

clean:
	rm -rf *.o
