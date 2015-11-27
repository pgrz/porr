#include <stdio.h>
#include <string.h>
#include "generator.h"

int main()
{
    char out[10];
    strcpy(out, "Output");

#pragma omp parallel
    {
        printf("%s\n", out);
    }

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
