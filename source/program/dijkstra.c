#include "dijkstra.h"
#include "utils.h"

#include <limits.h>

int *dijkstra_distance ( int ohd[vertex_count][vertex_count]  )
{
    int *connected;
    int i;
    int md;
    int *mind;
    int mv;
    int my_first;
    int my_id;
    int my_last;
    int my_md;
    int my_mv;
    int my_step;
    int nth;
    /*
       Start out with only node 0 connected to the tree.
     */
    connected = ( int * ) malloc ( vertex_count * sizeof ( int ) );

    connected[0] = 1;
    for ( i = 1; i < vertex_count; i++ )
    {
        connected[i] = 0;
    }
    /*
       Initial estimate of minimum distance is the 1-step distance.
     */
    mind = ( int * ) malloc ( vertex_count * sizeof ( int ) );

    for ( i = 0; i < vertex_count; i++ )
    {
        mind[i] = ohd[0][i];
    }
    /*
       Begin the parallel region.
     */
    # pragma omp parallel private ( my_first, my_id, my_last, my_md, my_mv, my_step ) \
    shared ( connected, md, mind, mv, nth, ohd )
    {
        my_id = omp_get_thread_num ( );
        nth = omp_get_num_threads ( );
        my_first =   (   my_id       * vertex_count ) / nth;
        my_last  =   ( ( my_id + 1 ) * vertex_count ) / nth - 1;
        /*
           The SINGLE directive means that the block is to be executed by only
           one thread, and that thread will be whichever one gets here first.
         */
        # pragma omp single
        {
            log(my_id, "P%d: Parallel region begins with %d threads", my_id, nth );
        }

        log(my_id, "P%d:  First=%d  Last=%d", my_id, my_first, my_last );

        for ( my_step = 1; my_step < vertex_count; my_step++ )
        {
            /*
               Before we compare the results of each thread, set the shared variable
               MD to a big value.  Only one thread needs to do this.
             */
            # pragma omp single
            {
                md = INT_MAX;
                mv = -1;
            }
            /*
               Each thread finds the nearest unconnected node in its part of the graph.
               Some threads might have no unconnected nodes left.
             */
            find_nearest ( my_first, my_last, mind, connected, &my_md, &my_mv, my_id );
            /*
               In order to determine the minimum of all the MY_MD's, we must insist
               that only one thread at a time execute this block!
             */
            # pragma omp critical
            {
                if ( my_md < md )
                {
                    md = my_md;
                    mv = my_mv;
                }
            }
            /*
               This barrier means that ALL threads have executed the critical
               block, and therefore MD and MV have the correct value.  Only then
               can we proceed.
             */
            # pragma omp barrier
            /*
               If MV is -1, then NO thread found an unconnected node, so we're done early.
               OpenMP does not like to BREAK out of a parallel region, so we'll just have
               to let the iteration run to the end, while we avoid doing any more updates.

               Otherwise, we connect the nearest node.
             */
            # pragma omp single
            {
                if ( mv != - 1 )
                {
                    connected[mv] = 1;
                    log(my_id, "P%d: Connecting node %d", my_id, mv );
                    log(-1, "----------------------------");
                }
            }
            /*
               Again, we don't want any thread to proceed until the value of
               CONNECTED is updated.
             */
            # pragma omp barrier
            /*
               Now each thread should update its portion of the MIND vector,
               by checking to see whether the trip from 0 to MV plus the step
               from MV to a node is closer than the current record.
             */
            if ( mv != -1 )
            {
                update_mind ( my_first, my_last, mv, connected, ohd, mind, my_id );
            }
            /*
               Before starting the next step of the iteration, we need all threads
               to complete the updating, so we set a BARRIER here.
             */
            #pragma omp barrier
        }
        /*
           Once all the nodes have been connected, we can exit.
         */
        # pragma omp single
        {
            log(my_id, "P%d: Exiting parallel region.", my_id );
        }
    }

    free ( connected );

    return mind;
}

void find_nearest ( int s, int e, int mind[vertex_count], int connected[vertex_count], int *d,
                    int *v, int my_id )
{
    int i;

    *d = INT_MAX;
    *v = -1;
    log(my_id, "P%d: initial distance: %d, initial closest: %d", my_id, *d, *v);

    for ( i = s; i <= e; i++ )
    {
        if ( !connected[i] && ( mind[i] < *d ) )
        {
            *d = mind[i];
            *v = i;
        }
        log(my_id, "P%d: after checked %d: current distance = %d, closest node=%d", my_id, i, *d, *v);
    }
    return;
}

void update_mind ( int s, int e, int mv, int connected[vertex_count], int ohd[vertex_count][vertex_count],
                   int mind[vertex_count], int my_id )
{
    int i;

    log(my_id, "P%d: updating mind", my_id);

    for ( i = s; i <= e; i++ )
    {
        if ( !connected[i] )
        {
            if ( ohd[mv][i] < INT_MAX )
            {
                if ( mind[mv] + ohd[mv][i] < mind[i] )
                {
                    mind[i] = mind[mv] + ohd[mv][i];
                }
            }
        }
    }
    return;
}
