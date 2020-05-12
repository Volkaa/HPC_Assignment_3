
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
    
    // Receiving every values from every other processes in a tree fashion
    for (int j = 1; j <= (int)(log2(size)); j++)
    {
        if (rank%((int)(pow(2,j))) == 0)
        {
            int temp;
            MPI_Recv(&temp, 1, MPI_INT, rank+(int)(pow(2,j-1)), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            count += temp;
        }
        else
        {
            //printf("Sent value from %d to %d\n", rank, rank-(int)(pow(2,j-1)));
            MPI_Send(&count, 1, MPI_INT, rank-(int)(pow(2,j-1)), 0, MPI_COMM_WORLD);
            break;
        }
    }
    
    t2 = MPI_Wtime();

    // Printing results
    if (rank == 0)
    {
        //Compute Pi
        pi = ((double)count / (double)NUM_ITER) * 4.0;

        printf("The result is %f\n", pi);
        printf("Total time : %f\n", t2 - t1);
    }

    MPI_Finalize();
    
    return 0;
}

