#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int tid,nprocs;
    char *cpuname;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &tid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    cpuname = (char*)calloc(80, sizeof(char));
    gethostname(cpuname,80);

    printf("Process %i of %i on machine %s\n", tid, nprocs, cpuname);

    MPI_Finalize();

    return 0;
}
