#ifndef __AUCTION_H__
#define __AUCTION_H__

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "generator.h"

int *auction_distance ( int ohd[vertex_count][vertex_count] );
void find_nearest_a ( int s, int e, int mind[vertex_count], int connected[vertex_count], int *d,
                    int *v );
void update_mind_a ( int s, int e, int mv, int connected[vertex_count], int ohd[vertex_count][vertex_count],
                   int mind[vertex_count] );


#endif //__AUCTION_H__
