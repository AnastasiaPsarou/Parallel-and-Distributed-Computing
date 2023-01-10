//Anastasia Psarou 2860
//Nikos Lymperopoulos 2812
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>


void function(void){
    int num_of_threads;
    double start, end, exec_time;

    printf("Enter number of threads to be used: ");
    scanf("%d", &num_of_threads);

    printf("\n");

    start = omp_get_wtime();

    printf("Number of processors: %d\n", omp_get_num_procs());
    printf("Max number of threads: %d\n", omp_get_max_threads());

    omp_set_num_threads(num_of_threads);

    #pragma omp parallel
    {
        #pragma omp master
        {

            printf("Num of threads: %d\n", omp_get_num_threads());

            printf("\n");
            printf("Hello. I am the master thread. \n");
            printf("\n");
        }

        #pragma omp barrier

        //edw tha mpoun ola ta nimata ektos apo to master
        if(omp_get_thread_num() != 0){
            printf("Hi there! I am thread %d\n", omp_get_thread_num());
        }
    }

    end = omp_get_wtime();
    exec_time = end - start;

    printf("Execution time is: %lf\n", exec_time);
}

int main(int argc, char *argv[]){
    
    function();

    return(0);
}