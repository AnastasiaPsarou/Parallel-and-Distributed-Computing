//Calculation of infinite norm
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include <unistd.h>

#define array_size 16384
double array[array_size][array_size];

double LoopFor_1(){
    int sum=0;
    clock_t start_t, end_t;
    double total_t;
    int max_sum = 0;

    start_t = clock(); 

    for(int i = 0; i < array_size; i++){
        for(int j = 0, sum = 0; j < array_size; j++){
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

    for(int i = 0; i < array_size; i++){
        for(int j = 0, sum = 0; j < array_size; j+=2){
            sum+= abs(array[i][j]);
            sum+= abs(array[i][j+1]);
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

    for(int i = 0; i < array_size; i++){
        for(int j = 0, sum = 0; j < array_size; j+=4){
            sum+= abs(array[i][j]);
            sum+= abs(array[i][j+1]);
            sum+= abs(array[i][j+2]);
            sum+= abs(array[i][j+3]);
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

    for(int i = 0; i < array_size; i++){
        for(int j = 0, sum = 0; j < array_size; j+=8){
            sum+= abs(array[i][j]);
            sum+= abs(array[i][j+1]);
            sum+= abs(array[i][j+2]);
            sum+= abs(array[i][j+3]);
            sum+= abs(array[i][j+4]);
            sum+= abs(array[i][j+5]);
            sum+= abs(array[i][j+6]);
            sum+= abs(array[i][j+7]);
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

    for(int i = 0; i < array_size; i++){
        for(int j = 0, sum = 0; j < array_size; j+=16){
            sum+= abs(array[i][j]);
            sum+= abs(array[i][j+1]);
            sum+= abs(array[i][j+2]);
            sum+= abs(array[i][j+3]);
            sum+= abs(array[i][j+4]);
            sum+= abs(array[i][j+5]);
            sum+= abs(array[i][j+6]);
            sum+= abs(array[i][j+7]);
            sum+= abs(array[i][j+8]);
            sum+= abs(array[i][j+9]);
            sum+= abs(array[i][j+10]);
            sum+= abs(array[i][j+11]);
            sum+= abs(array[i][j+12]);
            sum+= abs(array[i][j+13]);
            sum+= abs(array[i][j+14]);
            sum+= abs(array[i][j+15]);
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

    //Arxikopoiisi pinaka
    for(int i=0; i < array_size; i++){
        for(int j=0; j < array_size; j++){
            array[i][j]= ((i+1)*j*1.2345)/0.333;
        }
    }

    time_elapsed = LoopFor_16();
    printf("%f\n", time_elapsed);

    return(0);
}