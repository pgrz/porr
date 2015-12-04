CC=mpicc

SRCS=$(wildcard *.c)
DEPS =$(wildcard *.h)
OBJS=$(SRCS:.c=.o)

EXE=program
BUILDDIR=../build

CFLAGS  += 
LDFLAGS += 

all : $(EXE)
	$(info make $@ for $(EXE)...)

$(EXE) : $(OBJS)
	@$(CC) $^ $(LDFLAGS) -o $@

%.o : %.c $(DEPS)
	@$(CC) $(CFLAGS) -c $<

install:
	$(info make $@ for $(EXE)...)
	@cp $(EXE) $(BUILDDIR)

clean:
	$(info make $@ for $(EXE)...)
	@rm -f $(OBJS)

clobber:
	$(info make $@ for $(EXE)...)
	@rm -f $(OBJS) $(EXE)
