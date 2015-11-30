#include "generator.h"

int      edge_count=20;
int      vertex_count=10;
int      max_weight=20;
int      max_edges=0;
int     *adj_matrix;

void random_graph()
{
    int i, j, count, index, temp;
    int i4_huge = 2147483647;

    if ( ( adj_matrix = ( int * ) calloc( vertex_count * vertex_count, sizeof( int ) ) )
            == NULL ) {
        printf( "Not enough room for this size graph\n" );
        return;
    }

    for(i=0; i<vertex_count;i++)
        for(j=0;j<vertex_count;j++)
            adj_matrix[i * vertex_count + j] = i4_huge;

    for ( count = 0; count < edge_count; ) {
        i = rand()%vertex_count;
        j = rand()%vertex_count;

        if ( i == j )
            continue;

        if ( i > j )
        {
            temp = i;
            i = j;
            j = temp;
        }

        index = i * vertex_count + j;
        if ( adj_matrix[ index ] == i4_huge ) {
            adj_matrix[ index ] = 1 + rand()%max_weight;
            count++;
        }
    }

//    free( adj_matrix );
}

void print_graph()
{
    int i, j, index;

    printf( "%d\t%d\n", vertex_count, edge_count );

    for ( i = 0; i < vertex_count; i++ )
        for ( j = i + 1; j < vertex_count; j++ ) {
            index = i  * vertex_count + j ;
            if ( adj_matrix[ index ] )
                printf( "%d\t%d\t%d\n", i, j, adj_matrix[ index ] );
        }
}
