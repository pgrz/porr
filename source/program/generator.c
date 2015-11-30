#include "generator.h"
#include "utils.h"

#include <limits.h>

int      edge_count=20;
int      vertex_count=10;
int      max_weight=20;
int      max_edges=0;
int     *adj_matrix;

void random_graph()
{
    int i, j, count, index, temp;

    if ( ( adj_matrix = ( int * ) calloc( vertex_count * vertex_count, sizeof( int ) ) )
            == NULL ) {
        printf( "Not enough room for this size graph\n" );
        return;
    }

    for(i=0; i<vertex_count;i++)
        for(j=0;j<vertex_count;j++)
            adj_matrix[i * vertex_count + j] = INT_MAX;

    for ( count = 0; count < edge_count; ) {
        i = rand()%vertex_count;
        j = rand()%vertex_count;

        if ( i == j )
        {
            adj_matrix[i * vertex_count + j] = 0;
            continue;
        }
            
        if ( i > j )
        {
            temp = i;
            i = j;
            j = temp;
        }

        index = i * vertex_count + j;

        if ( adj_matrix[ index ] == INT_MAX ) {
            adj_matrix[ index ] = 1 + rand()%max_weight;
            count++;
        }
    }

//    free( adj_matrix );
}

void print_graph()
{
    int i, j, index;

    log(-1,  "Vertices: %d", vertex_count);
    log(-1,  "Edges:    %d", edge_count );
    log(-1,  "------------");

    for ( i = 0; i < vertex_count; i++ )
        for ( j = 0; j < vertex_count; j++ )
        {
            if(i==j) continue;
            index = i  * vertex_count + j ;
            if ( adj_matrix[ index ] != INT_MAX )
            log(-1, "%d --- %d : %d", i, j, adj_matrix[ index ] );
        }
}
