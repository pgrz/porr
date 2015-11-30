CC=gcc

SRCS=$(wildcard *.c)
DEPS =$(wildcard *.h)
OBJS=$(SRCS:.c=.o)

EXE=program

CFLAGS  += -fopenmp -g
LDFLAGS += -fopenmp -g

all : $(EXE)
	$(info make $@ for $(EXE)...)

$(EXE) : $(OBJS)
	@$(CC) $^ $(LDFLAGS) -o $@

%.o : %.c $(DEPS)
	@$(CC) $(CFLAGS) -c $<

clean:
	$(info make $@ for $(EXE)...)
	@rm -f $(OBJS)

clobber:
	$(info make $@ for $(EXE)...)
	@rm -f $(OBJS) $(EXE)
