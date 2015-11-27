#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX

#include "generator.h"
#include "dijkstra.h"

void usage(void)
{
    printf("Usage: program [VN] [EN] [MW]");
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

int str2int(const char* cc)
{
    char *p;

    errno = 0;

    long conv = strtol(cc, &p, 10);

    if (errno != 0 || *p != '\0' || conv > INT_MAX)
    {
        usage();
    }
    else
    {
        return conv;    
    }
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
        fprintf ( stdout, "\n" );
        fprintf ( stdout, "  Minimum distances from node 0:\n");
        fprintf ( stdout, "\n" );
        for ( i = 0; i < vertex_count; i++ )
        {
            fprintf ( stdout, "  %2d  %2d\n", i, mind[i] );
        }
        free ( mind );
    }
    else
    {
        printf("Graph not generated!\n");
    }

    return 0;
}
