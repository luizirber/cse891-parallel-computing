#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 100
#define SUM_TAG 2
#define DISTRIBUTE_TAG 3


int init_vector(int vect[]) {
    for (int i = 0; i < N; ++i ) {
      vect[i] = rand() % 100;
    }
    return 0;
}


int distribute_vector(int vect[], int procs) {
    int chunk_size = 0;
    for (int i = 1; i < procs; ++i ) {
      chunk_size = N / procs;
      MPI_Send(vect + i * chunk_size, chunk_size, MPI_INT, i, DISTRIBUTE_TAG, MPI_COMM_WORLD);
    }
    return 0;
}


int receive_vector(int vect[], int procs, int rank) {
    MPI_Status status;
    MPI_Recv(vect, N / procs, MPI_INT, 0, DISTRIBUTE_TAG, MPI_COMM_WORLD, &status);
    return 0;
}


int main(int argc, char **argv)
{
   int rank, procs;
   long long int my_sum = 0;
   long long int other_sum = 0;
   int vector[N];

   MPI_Status status;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &procs);

   if (rank == 0) {
      init_vector(vector);
      distribute_vector(vector, procs);

      my_sum = 0;
      for (int i = 0; i < N / procs; ++i ) {
         my_sum += vector[i];
      }
      printf("Rank 0 sum: %d\n", my_sum);

      for (int i = 1; i < procs; ++i ) {
         MPI_Recv(&other_sum, 1, MPI_LONG_LONG_INT, i, SUM_TAG, MPI_COMM_WORLD, &status);
         my_sum += other_sum;
         printf("Rank %d sum: %d | total: %d\n", i, other_sum, my_sum);
      }

      printf("Sum value: %d\n", my_sum);

      long long int serial_sum = 0;
      for (int i = 0; i < N; ++i)
        serial_sum += vector[i];

      if (serial_sum == my_sum) {
        printf("Serial == parallel\n");
      } else {
        printf("Serial != parallel\n");
      }

   } else {
      my_sum = 0;
      receive_vector(vector, procs, rank);

      for (int i = 0; i < N / procs; ++i ) {
         my_sum += vector[i];
      }

      MPI_Send(&my_sum, 1, MPI_LONG_LONG_INT, 0, SUM_TAG, MPI_COMM_WORLD);
   }

   MPI_Finalize();

   return(0);
}
