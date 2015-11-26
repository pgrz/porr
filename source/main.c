#include <stdio.h>

int main()
{
    #pragma omp parallel
    {
        printf("Duuuupaaa\n");
    }
    return 0;
}
