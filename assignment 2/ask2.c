#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>
#include <math.h>

#define NUM_OF_THREADS 32
#define N 100000000
#define CHUNK 1000

void simpson(){
    double sum = 0;
    int i;
    double start, end, total_time;
    int num_flops = 0;

    start = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum) schedule(dynamic, CHUNK)
    for(i = 0; i <= N; i++){
        if(i != 0 || i != (N-1)){
                if(i % 2 == 0){
                    sum += 4 * 4 / (1 + pow(i, 2) / pow(N, 2));
                    num_flops += 3;
                    continue;
                }
                else{
                    sum += 4 * 2 / (1 + pow(i, 2) / pow(N, 2));
                    num_flops += 3;
                    continue;
                }
        }
        else{
            sum += 4 / (1 + pow(i, 2) / pow(N, 2));
            num_flops += 3;
        }
    }

    sum = sum/(3*N);
    end = omp_get_wtime();
    total_time = end - start;


    printf("pi is approximately %.20lf , computations time = %lf, number of threads = %d, FLOPS = %lf, chunk = %d , scheduling = static\n", sum, total_time, NUM_OF_THREADS, num_flops/total_time, CHUNK);

    return;

}

int main(int argc, char *argv[]){

    omp_set_num_threads(NUM_OF_THREADS);

    simpson();

    return(0);
}