CLUSTER_LDFLAG=-L/shared/lib/igraph-0.7.1/build/lib
CLUSTER_CFLAG=-I/shared/lib/igraph-0.7.1/build/include/igraph

default: graph-cluster.o vertex-cluster.o edge-cluster.o
	gcc $(CLUSTER_CFLAG) $(CLUSTER_LDFLAG) -fopenmp -ligraph -o vertex_bt graph.o vertex_bt.o
	gcc $(CLUSTER_CFLAG) $(CLUSTER_LDFLAG) -fopenmp -ligraph -o edge_bt graph.o edge_bt.o

graph-cluster.o: graph.h graph.c
	gcc -c $(CLUSTER_CFLAG) $(CLUSTER_LDFLAG) -ligraph -g graph.c

vertex-cluster.o: graph.h vertex_bt.c
	gcc -c $(CLUSTER_CFLAG) $(CLUSTER_LDFLAG) -fopenmp -ligraph vertex_bt.c

edge-cluster.o: graph.h edge_bt.c
	gcc -c $(CLUSTER_CFLAG) $(CLUSTER_LDFLAG) -fopenmp -ligraph edge_bt.c

clean:
	rm -f *.o vertex_bt edge_bt 
