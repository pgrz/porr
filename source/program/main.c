#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "utils.h"
#include "generator.h"
#include "dijkstra.h"

void usage(void)
{
    log(-1, "Usage: program [VN] [EN] [MW]");
    exit(1);
}

void timestamp ( void )
{
# define TIME_SIZE 40

    static char time_buffer[TIME_SIZE];
    const struct tm *tm;
    size_t len;
    time_t now;

    now = time ( NULL );
    tm = localtime ( &now );

    len = strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

    printf ( "%s\n", time_buffer );

    return;
# undef TIME_SIZE
}

int main (int argc, char **argv )
{
    int *mind;

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

    //Generate
    random_graph();

    //Print it
    if(adj_matrix != 0)
    {
        print_graph();

        mind = dijkstra_distance ( adj_matrix );
        /*
           Print the results.
         */
        int i;
        log(-1, "Minimum distances from node 0:");
        for ( i = 0; i < vertex_count; i++ )
        {
            log(-1, "%2d  %2d", i, mind[i]);
        }
        free ( mind );
    }
    else
    {
        log(-1, "Graph not generated!");
    }

    return 0;
}
