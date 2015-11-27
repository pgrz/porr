SRC := $(wildcard *.c)
EXE=domp
include ../Makefile.template
override CFLAGS  += -fopenmp -I/usr/local/include/igraph
override LDFLAGS += -fopenmp -L/usr/local/lib -ligraph
