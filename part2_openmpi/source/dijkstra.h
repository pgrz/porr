#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "generator.h"

int *dijkstra_distance ( int ohd[vertex_count][vertex_count] );
void find_nearest ( int mind[vertex_count], int connected[vertex_count], int *my_min );
void update_mind ( int mv, int connected[vertex_count], int ohd[vertex_count][vertex_count], int mind[vertex_count] );


#endif //__DIJKSTRA_H__
