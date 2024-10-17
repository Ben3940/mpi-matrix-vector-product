#include "microtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

typedef float *Matrix;

Matrix createMatrix(int rows, int cols)
{
    Matrix M;

    M = (Matrix)malloc(rows * cols * sizeof(M[0]));
    if (M == 0)
        fprintf(stderr, "Matrix allocation failed in file %s, line %d\n", __FILE__,
                __LINE__);

    return M;
}

void freeMatrix(Matrix M)
{
    if (M)
        free(M);
}

void initMatrix(Matrix A, int rows, int cols)
{
    int i, j;

    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            A[i * cols + j] = 1.0 / (i + j + 2);
}

// This can be improved!
void matVecMult(Matrix A, Matrix B, Matrix C, int rows, int cols)
{
    int i, k;

    memset(C, 0, rows * sizeof(C[0]));

    for (k = 0; k < cols; k++)
        for (i = 0; i < rows; i++)
            C[i] += A[i * cols + k] * B[k];
}

int main(int args, char **argv)
{

    char greeting[100];
    int comm_sz;
    int my_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank != 0)
    {
        sprintf(greeting, "Greetings form process %d of %d!", my_rank, comm_sz);
        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        printf("Greeting from process %d of %d!\n", my_rank, comm_sz);
        for (int q = 1; q < comm_sz; q++)
        {
            MPI_Recv(greeting, 100, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", greeting);
        }
    }

    MPI_Finalize();
    return 0;
}