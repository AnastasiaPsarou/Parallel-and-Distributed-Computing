In the programming exercises the goal was to calculate the 1-norm of a n*n array A.
The 1-norm has to be calculated with the use of 2 loops with different step (k=1, 2, 4, 8, 16).
In exercise 6 the traversion of the array takes place from column to column and in exercise 7 from line to line.

We notice a difference in the execution times of these 2 programs. The reason being is that in C arrays are saved continuously in memory by line.
So, in exercise 6 memory needs to be accessed more times, something that delays the execution.
On the other hand, in exercise 7 we have more cache hits and so less accesses to memory, that are quite time consuming.