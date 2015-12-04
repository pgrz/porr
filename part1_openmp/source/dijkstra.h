#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "generator.h"

int *dijkstra_distance ( int ohd[vertex_count][vertex_count] );
void find_nearest ( int s, int e, int mind[vertex_count], int connected[vertex_count], int *d,
                    int *v, int my_id );
void update_mind ( int s, int e, int mv, int connected[vertex_count], int ohd[vertex_count][vertex_count],
                   int mind[vertex_count], int my_id );


#endif //__DIJKSTRA_H__
