CC=gcc
CFLAGS= -g -O0 -fopenmp -Wformat-security -std=gnu11 -pedantic
LDFLAGS= -g -fopenmp
SRC_PRO=main.c
SRC_GEN=graph_ge.c
OBJ_GEN=$(SRC_GEN:.c=.o)
OBJ_PRO=$(SRC_PRO:.c=.o)
EXE_PRO=program
EXE_GEN=generator



all: $(EXE_GEN) $(EXE_PRO)

install:
	@cp $(EXE_GEN) $(EXE_PRO) $(OUTDIR)
    
$(EXE_GEN): $(OBJ_GEN) 
	@$(CC) $(LDFLAGS) $(OBJ_GEN) -o $@

$(EXE_PRO): $(OBJ_PRO) 
	@$(CC) $(LDFLAGS) $(OBJ_PRO) -o $@

.cpp.o:
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -f $(OBJ_GEN) $(OBJ_PRO)

clobber:
	@rm -f $(OBJ_GEN) $(OBJ_PRO) $(EXE_GEN) $(EXE_BIN)
