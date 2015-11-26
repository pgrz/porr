#include <stdio.h>
#include <string.h>

int main()
{
    char out[10];
    strcpy(out, "Output");

    #pragma omp parallel
    {
        printf("%s\n", out);
    }
    return 0;
}
