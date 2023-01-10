//Anastasia Psarou
//Nikos Lymperopoulos
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define N 9999999

int main( int argc, char *argv[] ) {
    int numprocs, myid, i;
    double dx = 1 / (double)N;
    double num_flops = 0;
    double sum = 0;
    double final_sum;
    double *x;
    double *x_procs;
    double *y_procs;
    double communication_time = 0;
    double computation_time = 0;
    double start_time_com;
    double start_time_comp;
    double final_time, fin, final, final_communication;
    double pi, total_time;
    double number_flops;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    int thread_work = (N + 1)/numprocs;

    x_procs = (double *)malloc((thread_work) * sizeof(double));
    y_procs = (double *)malloc((thread_work) * sizeof(double));
    x = (double *)malloc((N+1) * sizeof(double));
    
    if(myid == 0){
        for(i = 0; i <= N; i++){
            x[i] = i * dx;
        }
    }

    //start counting communication time
    start_time_com = MPI_Wtime();

    MPI_Scatter(x, thread_work, MPI_DOUBLE, x_procs, thread_work, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    final = MPI_Wtime() - start_time_com;
    MPI_Reduce(&final, &communication_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    //stop counting communication time and start counting computation time
    start_time_comp = MPI_Wtime();

    if(myid == 0){
        y_procs[0] = 4/(1+ pow(x_procs[0], 2));
        sum += y_procs[0];

        for(i = 1; i < thread_work; i++){
            if(i % 2 == 0){
                y_procs[i] = 4 * 4 / (1 + pow(x_procs[i], 2));
                num_flops += 3;
                sum += y_procs[i];
                continue;
            }
            else{
                y_procs[i] = 4 * 2 / (1 + pow(x_procs[i], 2));
                num_flops += 3;
                sum += y_procs[i];
                continue;
            }
        }
    }
    else if (myid < numprocs - 1){
        y_procs[thread_work - 1] = 4/(1 + pow(x_procs[thread_work - 1], 2));
        sum += y_procs[thread_work - 1];

        for(i = 0; i <= thread_work; i++){
            if((i + myid * thread_work) % 2 == 0){
                y_procs[i] = 4 * 4 / (1 + pow(x_procs[i], 2));
                num_flops += 3;
                sum += y_procs[i];
                continue;
            }
            else{
                y_procs[i] = 4 * 2 / (1 + pow(x_procs[i], 2));
                num_flops += 3;
                sum += y_procs[i];
                continue;
            }
        }
    }
    else{
        for(i = 0; i < thread_work; i++){
            if((i + myid * thread_work) % 2 == 0){
                y_procs[i] = 4 * 4 / (1 + pow(x_procs[i], 2));
                num_flops += 3;
                sum += y_procs[i];
                continue;
            }
            else{
                y_procs[i] = 4 * 2 / (1 + pow(x_procs[i], 2));
                num_flops += 3;
                sum += y_procs[i];
                continue;
            }
        }

    }
    //stop counting computation time
    final_time = MPI_Wtime() - start_time_comp;
    MPI_Reduce(&final_time, &computation_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    //start counting communication time
    start_time_com = MPI_Wtime();

    MPI_Reduce(&sum, &final_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&num_flops, &number_flops, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    fin = MPI_Wtime() - start_time_com;
    MPI_Reduce(&fin, &final_communication, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    communication_time += final_communication;

    if(myid == 0){
        pi = final_sum * dx / 3;
        printf("pi is approximately %lf , computations time = %lf, communication time = %lf, number of tasks = %d, MFLOPS = %4.3lf\n", pi, computation_time, communication_time, numprocs, number_flops/computation_time/1000000);
    }

    MPI_Finalize();

    return(0);
}
