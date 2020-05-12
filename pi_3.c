
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
    
    // Receiving every values from every other processes
    if (rank == 0)
    {
        //Receive every count value
        MPI_Request* requests = (MPI_Request*)malloc((size-1)*sizeof(MPI_Request));
        int* temp = (int*)malloc((size-1)*sizeof(int));
        for (int i = 1; i < size; i++)
            MPI_Irecv(&(temp[i-1]), 1, MPI_INT, i, 0, MPI_COMM_WORLD, &(requests[i-1]));
        
        MPI_Waitall(size-1, requests, MPI_STATUSES_IGNORE);

        //Sum all values
        for (int i = 0; i < size-1; i++)
            count += temp[i];

        //Compute Pi
        pi = ((double)count / (double)NUM_ITER) * 4.0;
    } 
    // Sending count value to process 0
    else 
    {
        MPI_Request req;
        MPI_Isend(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
        MPI_Wait(&req, MPI_STATUSES_IGNORE);
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

