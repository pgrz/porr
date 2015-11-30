#include <time.h>
#include <limits.h>

#include "utils.h"
#include "generator.h"
#include "dijkstra.h"
#include "auction.h"

typedef enum {
AUCTION, DIJKSTRA
} Algorithm;

void usage(void)
{
    log(INFO, -1, "Usage: program VERTICES EDGES MAXWEIGHT [LOGLEVEL]");
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

    if(a == AUCTION)
    {
        log(INFO, -1, "Auction execution time: %f s", time_spent);
    }
    else
    {
        log(INFO, -1, "Dijkstra execution time: %f s", time_spent);
    }
    
    dist=mind[last];

    if (dist != INT_MAX)
    {
        log(INFO, -1, "Minimum distance from node 0 to node %d equals %d", last, dist);
    }
    else
    {
        log(INFO, -1, "Can't reach node %d from node 0!", last);
    }

    free ( mind );
}


int main (int argc, char **argv )
{
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
        if(il >= 0 && il <= 3)
        {
            level = il;
        }
    }

    //Seed init
    srand( ( unsigned short ) time( NULL ) );

    //Fix imbalanced graph
    max_edges = (vertex_count * ( vertex_count - 1 )) / 2;
    edge_count = (edge_count > max_edges) ?  max_edges : edge_count;

    random_graph();


    if(adj_matrix != 0)
    {
        print_graph();

        runAlgorithm(DIJKSTRA);
        runAlgorithm(AUCTION);

    }
    else
    {
        log(WARNING, -1, "Graph not generated!");
    }

    return 0;
}
