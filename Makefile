CC = gcc

CLUSTER_LDFLAG=-L/shared/lib/igraph-0.7.1/build/lib
CLUSTER_CFLAG=-I/shared/lib/igraph-0.7.1/build/include/igraph

override CFLAGS := -O2 -fopenmp -ligraph $(CLUSTER_CFLAG) $(CFLAGS)
override LDFLAGS := -fopenmp -ligraph $(CLUSTER_LDFLAG) $(LDFLAGS)

SRCDIR=bc
SRCS=graph.c vertex_bt.c edge_bt.c
OBJS=$(SRCS:.c=.o)

default: all

all: edge_bt vertex_bt

edge_bt: graph.o edge_bt.o
	$(CC) $(LDFLAGS) $^ -o $@

vertex_bt: graph.o vertex_bt.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o vertex_bt edge_bt 

.PHONY: all default clean
