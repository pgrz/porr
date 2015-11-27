#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void fix_imbalanced_graph( void );
void print_graph( 
                  
                  int* adj_matrix);
void random_graph();

int ran( int k );     /* customized random number generator */

int      edge_count=20;
int      vertex_count=10;
int      max_weight=20;

int main()
{
    srand( ( unsigned short ) time( NULL ) );

    fix_imbalanced_graph();
    random_graph();
    return 0;
}

void fix_imbalanced_graph( void )
{
    int  max_edges;

    max_edges = (vertex_count * ( vertex_count - 1 )) / 2;
    if (edge_count > max_edges)
        edge_count = max_edges;
}

void random_graph()
{
    int i, j, count, index, *adj_matrix, temp;

    if ( ( adj_matrix = ( int * ) calloc( vertex_count * vertex_count, sizeof( int ) ) )
            == NULL ) {
        printf( "Not enough room for this size graph\n" );
        return;
    }

    for ( count = 0; count < edge_count; ) {
        i = ran( vertex_count );
        j = ran( vertex_count );

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
            adj_matrix[ index ] = 1 + ran( max_weight );
            count++;
        }
    }
    print_graph( adj_matrix);

    free( adj_matrix );
}

int ran(int k)
{
    return rand() % k;
}

void print_graph(int* adj_matrix)
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
