#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int main(int argc, char *argv[])
{
    int tid,nprocs;
    char *cpuname;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &tid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    cpuname = (char*)calloc(80, sizeof(char));
    gethostname(cpuname,80);

    log_d(tid, "Successfully started job %i of %i on %s", tid, nprocs, cpuname);

    MPI_Finalize();

    log_d(tid, "Full stop in %i on %s", tid, cpuname);

    return 0;
}
