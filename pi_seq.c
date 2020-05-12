
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <float.h>
#include <limits.h>

#define SEED     921
#define NUM_ITER 16777216

double mysecond(){
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

int main(int argc, char* argv[])
{
    int count = 0;
    double x, y, z, pi, t1, t2;
    
    srand(SEED); // Important: Multiply SEED by "rank" when you introduce MPI!
    
    t1 = mysecond();
    // Calculate PI following a Monte Carlo method
    for (int iter = 0; iter < NUM_ITER; iter++)
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
    
    // Estimate Pi and display the result
    pi = ((double)count / (double)NUM_ITER) * 4.0;
    
    t2 = mysecond();

    printf("The result is %f\n", pi);
    printf("Execution time : %f\n", t2-t1);
    
    return 0;
}

