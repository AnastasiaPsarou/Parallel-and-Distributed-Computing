//Anastasia Psarou
//Nikos Lymperopoulos
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 256

int main( int argc, char *argv[] ) {
    int numprocs, myid, times = 0;
    int source, rc, count, c;
    double end_time, start_time;
    int i, dest, tag = 1;
    char* num;
    MPI_Status Stat;
    char buf[SIZE];
    char inmsg[SIZE];
    char send_proc_buffer[SIZE];

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if(myid == 0){
        start_time = MPI_Wtime();
        printf("Total number of threads is %d\n", numprocs);
        printf("Hello. This is the master node.\n");
        strcpy(buf, "Hello. This is the master node.");

        for(i = 1; i < numprocs; i++){
            rc = MPI_Send(buf, SIZE, MPI_BYTE, i, 0, MPI_COMM_WORLD);
        }
        while(1){
            if(times == (numprocs - 1)){
                break;
            }
            rc = MPI_Recv(&inmsg, SIZE, MPI_BYTE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &Stat);
            times++;
        }

        printf("%d nodes replied back to master node.\n", times);
    }
    else{
        rc = MPI_Recv(buf, SIZE, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &Stat);
        printf("Hello. This is node %d\n", myid);
        sprintf(send_proc_buffer, "Hello. This is node %d.", myid);

        rc = MPI_Send(&send_proc_buffer, SIZE, MPI_BYTE, 0, 1, MPI_COMM_WORLD);

    }

    if(myid == 0){
        end_time = MPI_Wtime();
        printf("Execution time of the program is %lf\n", end_time - start_time);
    }
    

    c = MPI_Get_count(&Stat, MPI_CHAR, &count);
    MPI_Finalize();

    return(0);
}