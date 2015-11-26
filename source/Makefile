CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=program
OUTDIR=../build


all: $(SOURCES) $(EXECUTABLE)

install:
	@cp $(EXECUTABLE) $(OUTDIR)
    
$(EXECUTABLE): $(OBJECTS) 
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -f *.o

clobber:
	@rm -f *.o program
