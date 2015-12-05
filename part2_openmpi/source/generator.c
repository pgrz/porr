#include "generator.h"
#include "utils.h"

#include <string.h>

#include <limits.h>

int      edge_count=20;
int      vertex_count=10;
int      max_weight=20;
int      max_edges=0;
int     *adj_matrix;

void random_graph()
{
    int i, j, count, weight;

    if ( ( adj_matrix = ( int * ) calloc( vertex_count * vertex_count, sizeof( int ) ) ) == NULL )
    {
        log_e(tid, "Not enough room for this size graph");
        return;
    }

    for(i=0; i<vertex_count; i++)
        for(j=0; j<vertex_count; j++)
        {
            if(i == j)
            {
                adj_matrix[i * vertex_count + j] = 0;
            }    
            else
            {
                adj_matrix[i * vertex_count + j] = INT_MAX;
            }
        }

    for ( count = 0; count < edge_count; ) {
        i = rand()%vertex_count;
        j = rand()%vertex_count;

        if ( i == j )
        {
            adj_matrix[i * vertex_count + j] = 0;
            continue;
        }

        if ( adj_matrix[ i * vertex_count + j ] == INT_MAX ) {
            weight= 1 + rand()%max_weight;
            adj_matrix[ i * vertex_count + j ] = weight;
            adj_matrix[ j * vertex_count + i ] = weight;
            count++;
        }
    }

//    free( adj_matrix );
}

void print_graph()
{
    int i, j, index;

    log_d(tid,  "Vertices: %d", vertex_count);
    log_d(tid,  "Edges:    %d", edge_count );
    log_d(tid,  "------------");

    //char format[1024];
    //char num[32];

    for ( i = 0; i < vertex_count; i++ )
    {
        for ( j = 0; j < vertex_count; j++ )
        {
            if(i==j)
            {
                continue;
            }

            index = i  * vertex_count + j ;
            if ( adj_matrix[ index ] != INT_MAX )
            {
                log_d(tid, "%d --- %d : %d", i, j, adj_matrix[ index ] );
            }
        }
    }
/*
    log_d(tid, "--------------------");
    for ( i = 0; i < vertex_count; i++ )
    {
        strcpy(format, "");
        for ( j = 0; j < vertex_count; j++ )
        {
            index = i  * vertex_count + j ;

            if ( adj_matrix[ index ] != INT_MAX )
            {
                sprintf(num, "%d\t", adj_matrix[index]);
                strcat(format, num );
            }
            else
            {
                strcat(format, "Inf\t" );
            }

        }
        log_d(tid, "%s", format );
    }
    */
}
