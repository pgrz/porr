#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <limits.h>  // for INT_MAX
#include <time.h>

#include "utils.h"
#include "generator.h"
#include "dijkstra.h"
#include "auction.h"

int valid=0;
double dijk_time=0.0;
double auct_time=0.0;
int dijk_dist=0;
int auct_dist=0;
int all_valid=0;

typedef enum {
AUCTION, DIJKSTRA
} Algorithm;

void usage(void)
{
    log_i(tid, "Usage: program VERTICES EDGES MAXWEIGHT [LOGLEVEL]");
    MPI_Finalize();
    exit(1);
}

void runAlgorithm(Algorithm a)
{
    int *mind;
    clock_t beg, end;
    double time_spent;
    int last, dist;

    last=vertex_count - 1;

    beg = clock();

    if(a == AUCTION)
    {
        mind = auction_distance ( adj_matrix, 0 );
    }
    else
    {
        mind = dijkstra_distance ( adj_matrix );
    }

    end = clock();
    time_spent = (double)(end - beg) / CLOCKS_PER_SEC;

    dist = mind[last];
    if(dist != INT_MAX)
    {
        valid=1;
    }

    if (tid == 0)
    {
        if(a == AUCTION)
        {
            log_i(tid, "Auction execution time: %f s", time_spent);
            auct_time=time_spent;
            auct_dist=dist;
        }
        else
        {
            log_i(tid, "Dijkstra execution time: %f s", time_spent);
            dijk_time=time_spent;
            dijk_dist=dist;
        }

        if (dist != INT_MAX)
        {
            log_i(tid, "Minimum distance from node 0 to node %d equals %d", last, dist);
        }
        else
        {
            log_i( tid,  "Can't reach node %d from node 0!", last);
        }
    }

    free ( mind );
}

int main (int argc, char **argv )
{
    MPI_Init(&argc, &argv);

    MPI_Status stat;
    MPI_Comm_rank(MPI_COMM_WORLD, &tid);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    if (argc < 4 || argc > 5)
    {
        usage();
    }

    vertex_count=str2int(argv[1]);
    edge_count=str2int(argv[2]);
    max_weight=str2int(argv[3]);
    
    if(argc == 5)
    {
        int il = str2int(argv[4]);
        if((il >= ERROR && il <= DEBUG) || (il == TEST))
        {
            level = il;
        }
    }

    
    if ( ( adj_matrix = ( int * ) calloc( vertex_count * vertex_count, sizeof( int ) ) ) == NULL )
    {
        log_e(tid, "Not enough room for this size graph");

        //TODO MPI FATAL ERROR
        exit(2);
    }
    
    if(tid == 0)
    {
        //Seed init
        srand( ( unsigned short ) time( NULL ) );
        //srand( ( unsigned short ) 889 );

        //Fix imbalanced graph
        max_edges = (vertex_count * ( vertex_count - 1 )) / 2;
        edge_count = (edge_count > max_edges) ?  max_edges : edge_count;

        random_graph();

        print_graph();
    }
   
    log_i(tid, "Broadcasting adj_matrix...");
    MPI_Bcast(&adj_matrix[0], vertex_count * vertex_count, MPI_INT, 0, MPI_COMM_WORLD);
    log_i(tid, "Done!");
    MPI_Barrier( MPI_COMM_WORLD );

    if(adj_matrix != 0)
    {
        runAlgorithm(DIJKSTRA);
        if(valid > 0)
        {
            MPI_Barrier( MPI_COMM_WORLD );
            runAlgorithm(AUCTION);
            (tid == 0) ? log_t(-1,"%d\t%d\t%d\t%d\t%d\t%f\t%f", vertex_count, edge_count, max_weight, dijk_dist, auct_dist, dijk_time, auct_time) : 0 ;
        }
    }
    else
    {
        log_i( tid, "Graph not generated!");
    }

    MPI_Finalize();

    return 0;
}
