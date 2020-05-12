
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define SEED     921
#define NUM_ITER 16777216 // = 2^24

int main(int argc, char* argv[])
{
    int count = 0, size, rank, provided, iter;
    double x, y, z, pi, t1, t2;
    
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    t1 = MPI_Wtime();

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(SEED*rank); // Important: Multiply SEED by "rank" when you introduce MPI!
    
    // Calculate PI following a Monte Carlo method
    for (iter = 0; iter < NUM_ITER/size; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x*x) + (y*y));
        
        // Check if point is in unit circle
        if (z <= 1.0)
        {
            count++;
        }
    }
    
    int shared_sum = 0;
    MPI_Win win;

    MPI_Win_create(&shared_sum, 1, MPI_INT, MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    
    MPI_Win_fence(0, win);

    // Receiving every values from every other processes
    MPI_Accumulate(&count, 1, MPI_INT, 0, 0, 1, MPI_INT, MPI_SUM, win);

    MPI_Win_fence(0, win);

    MPI_Win_free(&win);

    if (rank == 0)
    {
        //Compute Pi
        pi = ((double)shared_sum / (double)NUM_ITER) * 4.0;
    } 
    
    t2 = MPI_Wtime();

    // Printing results
    if (rank == 0)
    {
        printf("The result is %f\n", pi);
        printf("Total time : %f\n", t2 - t1);
    }

    MPI_Finalize();
    
    return 0;
}

