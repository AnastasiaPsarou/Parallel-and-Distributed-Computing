#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>
#include <math.h>

#define N 1000
#define M 20
#define NUM_OF_THREADS 32

int main(int argc, char *argv[])
{
    double **A;
    double **D_ant;
    double **L_plus_U;
    double *b;
    int i, j, k, l;
    double *x_old;
    double *x_new;
    double *D_b;
    double **D_L_plus_U;
    double *help_array;
    double residual, difference;
    double start, end, total_time;

    //memory allocation for the arrays
    A = (double **)malloc(N * sizeof(double *));
    L_plus_U = (double **)malloc(N * sizeof(double *));
    D_ant = (double **)malloc(N * sizeof(double *));
    D_L_plus_U = (double **)malloc(N * sizeof(double *));

    x_old = (double *)malloc(N * sizeof(double));
    x_new = (double *)malloc(N * sizeof(double));
    b = (double *)malloc(N * sizeof(double));
    D_b = (double *)malloc(N * sizeof(double));
    help_array = (double *)malloc(N * sizeof(double));

    for (i = 0; i < N; i++)
    {
        A[i] = (double *)malloc(N * sizeof(double));
        L_plus_U[i] = (double *)malloc(N * sizeof(double));
        D_ant[i] = (double *)malloc(N * sizeof(double));
        D_L_plus_U[i] = (double *)malloc(N * sizeof(double));
    }

    //initialization of the arrays
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            D_L_plus_U[i][j] = 0;
            if (i == j)
            {
                D_ant[i][j] = 0.5;
                L_plus_U[i][j] = 0;
            }
            else if ((j == i + 1) || (j == i - 1))
            {
                L_plus_U[i][j] = -1;
                D_ant[i][j] = 0;
            }
            else
            {
                D_ant[i][j] = 0;
                L_plus_U[i][j] = 0;
            }
        }
    }

    for (i = 0; i < N; i++)
    {
        b[i] = 0;
        help_array[i]=0;
        x_old[i] = 0;
        x_new[i] = 0;
    }

    b[N - 1] = N + 1;
    omp_set_num_threads(NUM_OF_THREADS);

    //calculation of D^(-1)*D_b
    #pragma omp parallel for
    for (j = 0; j < N; j++)
    {
        D_b[j] = b[j] * D_ant[j][N - 1];
    }

    //calculation of D^(-1)*(L+U)
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            for (k = 0; k < N; k++)
            {
                D_L_plus_U[i][j] += ((D_ant[i][k]) * L_plus_U[k][j]);
            }
        }
    }

    double sum = 0;

    start = omp_get_wtime();
    for (int counter = 0; counter < M; counter++)
    {   
        #pragma omp parallel for reduction(+:sum)
        for (i = 0; i < N; i++)
        {
            sum = 0;
            for (j = 0; j < N; j++)
            {
                sum += D_L_plus_U[i][j] * x_old[j];
            }
            x_new[i] = D_b[i] - sum;
        }

        // calculate residual, difference
        #pragma omp for private(l)
        for (k=0; k < N; k++) {
           
            for (l=0; l < N; l++)
                help_array[k] += A[k][l]*x_new[l];

            help_array[k] = pow(b[k]- help_array[k], 2);

            #pragma omp critical (residual)
            {
                residual += help_array[k];
            }

            help_array[k] = pow(x_old[k]-x_new[k], 2);
            #pragma omp critical (difference)
            {
                difference += help_array[k];
            }

        }

        #pragma omp parallel for
        for (i = 0; i < N; i++)
        {   
            x_old[i] = x_new[i];
            x_new[i] = 0;
        }

        printf("iter = %d, residual = %lf, difference = %lf\n", counter, residual, difference);
    }
    end = omp_get_wtime();
    total_time = end - start;

    printf("Total time is %lf\n", total_time);

    //deallocation of memory used
    for (i = 0; i < N; i++)
    {
        free(A[i]);
        free(L_plus_U[i]);
        free(D_ant[i]);
        free(D_L_plus_U[i]);
    }

    free(A);
    free(D_L_plus_U);
    free(L_plus_U);
    free(D_ant);
    free(x_old);
    free(x_new);
    free(b);
    free(D_b);

    return (0);
}