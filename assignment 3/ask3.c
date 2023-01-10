//Anastasia Psarou
//Nikos Lymperopoulos
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define N 1024
#define M 50

int main(int argc, char *argv[]){
    int numprocs, myid;
    double **A;
    double *b;
    double *x_old;
    double *x_new;
    double *b_procs, *x_old_procs, *x_new_procs;
    int i, m;
    double residual, difference, total_residual, total_difference;
    int *sendcounts, *displs;
    double start_time_com;
    double start_time_comp;
    double final_time, fin, final, final_communication, final_computation;
    double communication_time = 0;
    double computation_time = 0;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    int thread_work = N / numprocs;
    
    x_new_procs = (double *)malloc((thread_work+2) * sizeof(double));
    b_procs = (double *)malloc(thread_work * sizeof(double));
    x_old_procs = (double *)malloc((thread_work+2) * sizeof(double));

    if(myid == 0){
        x_old = (double *)malloc(N * sizeof(double));
        x_new = (double *)malloc(N * sizeof(double));
        b = (double *)malloc(N * sizeof(double));
        sendcounts = (int *)malloc(numprocs * sizeof(int));
        displs = (int *)malloc(numprocs * sizeof(int));

        //arrays initialization
        for (i = 0; i < N; i++){
            if(i < (thread_work + 2)){
                x_old_procs[i] = 0;
                x_new_procs[i] = 0;
            }
            if(i < thread_work){
                b_procs[i] = 0;
            }
            b[i] = 0;
            x_old[i] = 1;
            x_new[i] = 0;
            
        }

        b[N - 1] =(double)N + 1;

        displs[0]=0;

        if(numprocs == 1){
            sendcounts[0]=N;
        }
        else if(numprocs == 2){
            sendcounts[0]=N/2+1;
            sendcounts[1]=N/2+1;

            displs[1]=N/2-1;
        }
        else{
            sendcounts[0]=thread_work+1;
            sendcounts[numprocs - 1] = thread_work + 1;
            displs[numprocs - 1] = N - thread_work - 1;

            for(i = 1; i <= numprocs - 2; i++){
                sendcounts[i] = thread_work + 2;
                displs[i] = i * thread_work - 1;
            }            
        }
    }

    //start counting communication time
    start_time_com = MPI_Wtime();

    MPI_Scatter(b, thread_work, MPI_DOUBLE, b_procs, thread_work, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    final = MPI_Wtime() - start_time_com;
    MPI_Reduce(&final, &communication_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
   
    for(m = 0; m < M; m++){
        //start counting communication time
        start_time_com = MPI_Wtime();

        MPI_Scatterv(x_old, sendcounts, displs, MPI_DOUBLE, x_old_procs, thread_work + 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        fin = MPI_Wtime() - start_time_com;
        MPI_Reduce(&fin, &final_communication, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        communication_time += final_communication;

        //stop counting communication time and start counting computation time
        start_time_comp = MPI_Wtime();
        residual = 0;
        difference = 0;

        //find x_new
        if(myid == 0){
            x_new_procs[0] = 0.5 * (x_old_procs[1] + b_procs[0]);
            for(i = 1; i <= thread_work - 1; i++){
                x_new_procs[i] = 0.5 * (x_old_procs[i - 1] + x_old_procs[i + 1] + b_procs[i]);
            }
        }
        else if(myid < numprocs - 1){
            for(i = 0; i < thread_work; i++){
                x_new_procs[i] = (0.5) * (x_old_procs[i] + x_old_procs[i+2] + b_procs[i]);
            }
        }
        else{
            
            x_new_procs[thread_work - 1] = (0.5)*(x_old_procs[thread_work-2] + b_procs[thread_work-1]);

            for(i = 0; i <= thread_work - 2; i++){
                x_new_procs[i] = (0.5) * (x_old_procs[i] + x_old_procs[i+2] + b_procs[i]);
            }

        }      

        //start counting communication time and stop counting computation
        final_time = MPI_Wtime() - start_time_comp;
        MPI_Reduce(&final_time, &final_computation, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        computation_time += final_computation;

        start_time_com = MPI_Wtime();

        //collect x_new
        MPI_Gather(x_new_procs, thread_work, MPI_DOUBLE, x_new, thread_work, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatterv(x_new, sendcounts, displs, MPI_DOUBLE, x_new_procs, thread_work+2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        fin = MPI_Wtime() - start_time_com;
        MPI_Reduce(&fin, &final_communication, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        communication_time += final_communication;

         //stop counting communication time and start counting computation time
        start_time_comp = MPI_Wtime();

        //find difference
        for(int k = 0; k < thread_work; k++){
            difference += pow(x_old_procs[k] - x_new_procs[k], 2);
        }

        //find residual
        if(myid == 0){
            residual += fabs(2 * x_new_procs[0] - x_new_procs[1] - b_procs[0]);

            for(i = 1; i <= thread_work - 1; i++){
                residual += fabs(2 * x_new_procs[i] - x_new_procs[i + 1] - x_new_procs[i +-1] - b_procs[0]);
            }
        }
       else if(myid < numprocs-1){
           for(i = 0; i < thread_work; i++)
                residual += fabs(-x_new_procs[i] + 2 *x_new_procs[i + 1] - x_new_procs[i + 2] - b_procs[i]);
        }
        else{
            residual += fabs(-x_new_procs[thread_work - 1] + 2 * x_new_procs[thread_work] - b_procs[thread_work - 1]);

            for(i = 0; i < thread_work - 1; i++)
                residual += fabs(-x_new_procs[i] + 2 * x_new_procs[i + 1] - x_new_procs[i + 2] -b_procs[i]);
        }
      
        final_time = MPI_Wtime() - start_time_comp;
        MPI_Reduce(&final_time, &final_computation, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        computation_time += final_computation;

        start_time_com = MPI_Wtime();

        MPI_Scatter(x_old, thread_work, MPI_DOUBLE, x_old_procs, thread_work, MPI_DOUBLE, 0,MPI_COMM_WORLD);
        MPI_Scatter(x_new, thread_work, MPI_DOUBLE, x_new_procs, thread_work, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //collect x_old from x
        MPI_Gather(x_new_procs, thread_work, MPI_DOUBLE, x_old, thread_work, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //find total difference and total residual
        MPI_Reduce(&residual, &total_residual, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&difference, &total_difference, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        //stop counting time for communication
        fin = MPI_Wtime() - start_time_com;
        MPI_Reduce(&fin, &final_communication, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        communication_time += final_communication;

        if(myid == 0){
            printf("iter = %d, residual = %lf, difference = %lf\n", m, total_residual, total_difference);
        }
    }

    if(myid == 0){
        printf("computation time = %lf sec, communication time = %lf sec\n", computation_time, communication_time);

        free(x_old);
        free(x_new);
        free(b);
        free(sendcounts);
        free(x_new_procs);
        free(x_old_procs);
        free(displs);

        free(b_procs);
    }

    MPI_Finalize();

    return(0);
}