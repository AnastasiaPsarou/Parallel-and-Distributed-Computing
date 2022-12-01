#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include <unistd.h>
#include "ex6.h"

#define array_size 16384
double array[array_size][array_size];

double LoopFor_1(){
    int sum=0;
    clock_t start_t, end_t;
    double total_t;
    int max_sum = 0;

    start_t = clock(); 

    for(int j = 0; j < array_size; j++){
        for(int i = 0, sum = 0; i < array_size; i++){
            sum += abs(array[i][j]);
        }
        if(max_sum < sum){
            max_sum = sum;
        }
    }

    end_t = clock(); 
    
    total_t = ((double)(end_t - start_t)) / CLOCKS_PER_SEC;
   
    return(total_t);

}
double LoopFor_2(){
    int sum=0;
    clock_t start_t, end_t;
    double total_t;
    int max_sum = 0;

    start_t = clock(); 

    for(int j = 0; j < array_size; j++){
        for(int i=0, sum = 0; i < array_size; i+=2){
            sum+= abs(array[i][j]);
            sum+= abs(array[i+1][j]);
        }
        if(max_sum < sum){
            max_sum = sum;
        }
    }

    end_t = clock(); 
    
    total_t = ((double)(end_t - start_t)) / CLOCKS_PER_SEC;
   
    return(total_t);

}
double LoopFor_4(){
    int sum=0;
    clock_t start_t, end_t;
    double total_t;
    int max_sum = 0;

    start_t = clock(); 

    for(int j = 0; j < array_size; j++){
        for(int i=0, sum = 0; i < array_size; i+=4){
            sum+= abs(array[i][j]);
            sum+= abs(array[i+1][j]);
            sum+= abs(array[i+2][j]);
            sum+= abs(array[i+3][j]);
        }
        if(max_sum < sum){
            max_sum = sum;
        }
    }

    end_t = clock(); 
    
    total_t = ((double)(end_t - start_t)) / CLOCKS_PER_SEC;
   
    return(total_t);

}
double LoopFor_8(){
    int sum=0;
    clock_t start_t, end_t;
    double total_t;
    int max_sum = 0;

    start_t = clock(); 

    for(int j = 0; j < array_size; j++){
        for(int i=0, sum = 0; i < array_size; i+=8){
            sum+= abs(array[i][j]);
            sum+= abs(array[i+1][j]);
            sum+= abs(array[i+2][j]);
            sum+= abs(array[i+3][j]);
            sum+= abs(array[i+4][j]);
            sum+= abs(array[i+5][j]);
            sum+= abs(array[i+6][j]);
            sum+= abs(array[i+7][j]);
        }
        if(max_sum < sum){
            max_sum = sum;
        }
    }

    end_t = clock(); 
    
    total_t = ((double)(end_t - start_t)) / CLOCKS_PER_SEC;
   
    return(total_t);

}
double LoopFor_16(){
    int sum=0;
    clock_t start_t, end_t;
    double total_t;
    int max_sum = 0;

    start_t = clock(); 

    for(int j = 0; j < array_size; j++){
        for(int i = 0, sum = 0; i < array_size; i+=16){
            sum+= abs(array[i][j]);
            sum+= abs(array[i+1][j]);
            sum+= abs(array[i+2][j]);
            sum+= abs(array[i+3][j]);
            sum+= abs(array[i+4][j]);
            sum+= abs(array[i+5][j]);
            sum+= abs(array[i+6][j]);
            sum+= abs(array[i+7][j]);
            sum+= abs(array[i+8][j]);
            sum+= abs(array[i+9][j]);
            sum+= abs(array[i+10][j]);
            sum+= abs(array[i+11][j]);
            sum+= abs(array[i+12][j]);
            sum+= abs(array[i+13][j]);
            sum+= abs(array[i+14][j]);
            sum+= abs(array[i+15][j]);
        }
        if(max_sum < sum){
            max_sum = sum;
        }
    }

    end_t = clock(); 
    
    total_t = ((double)(end_t - start_t)) / CLOCKS_PER_SEC;
   
    return(total_t);

}

int main(int argc, char *argv[]){
    double time_elapsed;

    //Array initialisation
    for(int i=0; i < array_size; i++){
        for(int j=0; j < array_size; j++){
            array[i][j]= ((i+1)*j*1.2345)/0.333;
        }
    }

    time_elapsed = LoopFor_16();
    printf("%f\n", time_elapsed);

    return(0);
}