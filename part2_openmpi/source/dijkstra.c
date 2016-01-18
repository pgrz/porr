#include "dijkstra.h"
#include "utils.h"

#include <limits.h>

int my_first, my_last;

int *dijkstra_distance ( int ohd[vertex_count][vertex_count] )
{
    int *connected;
    int i;
    int my_min[2];
    int all_min[2];
    int *mind, *commonmind;
    int nth;
    int rc;

    connected = ( int * ) malloc ( vertex_count * sizeof ( int ) );

    connected[0] = 1;
    for ( i = 1; i < vertex_count; i++ )
    {
        connected[i] = 0;
    }

    mind = ( int * ) malloc ( vertex_count * sizeof ( int ) );
    commonmind = ( int * ) malloc ( vertex_count * sizeof ( int ) );

    for ( i = 0; i < vertex_count; i++ )
    {
        mind[i] = ohd[0][i];
    }

    my_first =   (   tid       * vertex_count ) / ntasks;
    my_last  =   ( ( tid + 1 ) * vertex_count ) / ntasks - 1;

    log_d(tid, "First=%d  Last=%d", my_first, my_last );

    for ( i = 1; i < vertex_count; i++ )
    {
        my_min[0] = INT_MAX;
        my_min[1] = -1;

        find_nearest ( mind, connected, my_min );

        if(MPI_Allreduce(my_min, all_min, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD) != MPI_SUCCESS)
        {
            log_e(tid, "MPI_Allreduce failed!");
        }

        tid == 0 ? log_i(tid, "Common minimal node %d with distance %d", all_min[1], all_min[0]) : 0 ;

        if ( all_min[1] != -1 )
        {
            tid == 0 ? log_d(tid, "Connecting node %d", all_min[1]) : 0 ;

            connected[all_min[1]] = 1;
            update_mind ( all_min[1], connected, ohd, mind);
        }
        
        if(MPI_Allreduce(mind, commonmind, vertex_count, MPI_INT, MPI_MIN, MPI_COMM_WORLD) != MPI_SUCCESS)
        {
            log_e(tid, "MPI_Allreduce failed!");
        }

        for ( i = 0; i < vertex_count; i++ )
        {
            mind[i] = commonmind[i];
        }
    }

    free ( connected );
    free ( mind );

    return commonmind;
}

void find_nearest ( int mind[vertex_count], int connected[vertex_count], int *my_min )
{
    int i;

    my_min[0] = INT_MAX;
    my_min[1] = -1;

    for ( i = my_first; i <= my_last; i++ )
    {
        if ( !connected[i] && ( mind[i] < my_min[0] ) )
        {
            my_min[0] = mind[i];
            my_min[1] = i;
        }
        log_d(tid, "After checked %d: current distance = %d, closest node=%d", i, my_min[0], my_min[1]);
    }
    return;
}

void update_mind ( int mv, int connected[vertex_count], int ohd[vertex_count][vertex_count], int mind[vertex_count])
{
    int i;

    log_d(tid, "Updating mind");

    for ( i = my_first; i <= my_last; i++ )
    {
        if ( !connected[i] )
        {
            if ( ohd[mv][i] < INT_MAX )
            {
                if ( mind[mv] + ohd[mv][i] < mind[i] )
                {
                    log_i(tid, "Updating mind[%d] from %d to %d", i, mind[i], mind[mv] + ohd[mv][i]);
                    mind[i] = mind[mv] + ohd[mv][i];
                }
            }
        }
    }
    return;
}
