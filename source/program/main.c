#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include "utils.h"
#include "generator.h"
#include "dijkstra.h"

void usage(void)
{
    log(-1, "Usage: program [VN] [EN] [MW]");
    exit(1);
}

int main (int argc, char **argv )
{
    int *mind;
    clock_t beg, end;
    double time_spent;
    int last, dist;

    if (argc != 4)
    {
        usage();
    }

    vertex_count=str2int(argv[1]);
    edge_count=str2int(argv[2]);
    max_weight=str2int(argv[3]);

    //Seed init
    srand( ( unsigned short ) time( NULL ) );

    //Fix imbalanced graph
    max_edges = (vertex_count * ( vertex_count - 1 )) / 2;
    edge_count = (edge_count > max_edges) ?  max_edges : edge_count;

    random_graph();


    if(adj_matrix != 0)
    {
        print_graph();

        beg = clock();
        mind = dijkstra_distance ( adj_matrix );
        end = clock();
        time_spent = (double)(end - beg) / CLOCKS_PER_SEC;

        log(-1, "Execution time: %f s", time_spent);
        
        last=vertex_count - 1;
        dist=mind[last];

        if (dist != INT_MAX)
        {
            log(-1, "Minimum distance from node 0 to node %d equals %d", last, dist);
        }
        else
        {
            log(-1, "Can't reach node %d from node 0!", last);
        }

        free ( mind );
    }
    else
    {
        log(-1, "Graph not generated!");
    }

    return 0;
}
