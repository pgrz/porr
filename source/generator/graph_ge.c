#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_graph();
void random_graph();

int      edge_count=20;
int      vertex_count=10;
int      max_weight=20;
int      max_edges=0;
int     *adj_matrix;

int main()
{
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
    }
    else
    {
        printf("Graph not generated!\n");
    }

    return 0;
}

void random_graph()
{
    int i, j, count, index, temp;
    return;

    if ( ( adj_matrix = ( int * ) calloc( vertex_count * vertex_count, sizeof( int ) ) )
            == NULL ) {
        printf( "Not enough room for this size graph\n" );
        return;
    }

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
        if ( !adj_matrix[ index ] ) {
            adj_matrix[ index ] = 1 + rand()%max_weight;
            count++;
        }
    }

    free( adj_matrix );
}

void print_graph()
{
    int i, j, index;

    printf( "%d\t%d\n", vertex_count, edge_count );

    for ( i = 1; i < vertex_count; i++ )
        for ( j = i + 1; j <= vertex_count; j++ ) {
            index = ( i - 1 ) * vertex_count + j - 1;
            if ( adj_matrix[ index ] )
                printf( "%d\t%d\t%d\n", i, j, adj_matrix[ index ] );
        }
}
