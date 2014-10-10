#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#ifndef VECTOR_SIZE
#define VECTOR_SIZE 100
#endif

#define DISTRIBUTE_TAG 3
#define SUM_BASE_STEP_TAG 4


int calc_chunk_for_proc(int proc, int problem_size, int procs) {
  int chunk_size = problem_size / procs;
  if (proc == procs - 1) {
    chunk_size = problem_size - proc * chunk_size;
  }
  return chunk_size;
}


int init_vector(int vect[]) {
  for (int i = 0; i < VECTOR_SIZE; ++i ) {
    vect[i] = rand() % 100;
    //vect[i] = i;
  }
  return 0;
}


int distribute_vector(int vect[], int procs) {
  int chunk_size = 0;
  for (int i = 1; i < procs; ++i ) {
    chunk_size = calc_chunk_for_proc(i, VECTOR_SIZE, procs);
    MPI_Send(vect + i * (VECTOR_SIZE / procs), chunk_size, MPI_INT, i, DISTRIBUTE_TAG, MPI_COMM_WORLD);
  }
  return 0;
}


int receive_vector(int vect[], int chunk_size) {
  MPI_Status status;
  MPI_Recv(vect, chunk_size, MPI_INT, 0, DISTRIBUTE_TAG, MPI_COMM_WORLD, &status);
  return 0;
}


int sum_vector(int vector[], int chunk_size) {
  long long int my_sum = 0;
  for (int i = 0; i < chunk_size; ++i ) {
    my_sum += vector[i];
  }
  return my_sum;
}


int main(int argc, char **argv) {
  int rank, procs;
  long long int my_sum = 0;
  long long int total_sum = 0;
  int vector[VECTOR_SIZE];
  int chunk_size;

  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);

  chunk_size = calc_chunk_for_proc(rank, VECTOR_SIZE, procs);
  if (rank == 0) {
    init_vector(vector);
    distribute_vector(vector, procs);
  } else {
    receive_vector(vector, chunk_size);
  }

  my_sum = sum_vector(vector, chunk_size);

  MPI_Reduce(&my_sum, &total_sum, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  /*
  if (rank == 0) {
    long long int serial_sum = 0;
    for (int i = 0; i < VECTOR_SIZE; ++i)
      serial_sum += vector[i];

    if (serial_sum == total_sum) {
      printf("DEBUG: Serial == parallel\n");
    } else {
      printf("DEBUG: Serial != parallel, %d\n", abs(serial_sum - my_sum));
    }
  }
  */

  MPI_Finalize();

  return(0);

}
