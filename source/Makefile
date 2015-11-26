CC=gcc
CFLAGS= -g -O0 -fopenmp
LDFLAGS= -g -fopenmp
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=program



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
	@rm -f *.o $(EXECUTABLE)
