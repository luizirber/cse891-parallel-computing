#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef VECTOR_SIZE
#define VECTOR_SIZE 100
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

void count_sort(int a[], int n) {
  int i, j, count;
  int *temp = malloc(n*sizeof(int));

  for (i = 0; i < n; i++) {
    count = 0;
    #pragma omp parallel for default(none) private(j) shared(count, i, n, a)
    for (j = 0; j < n; j++) {
      if (a[j] < a[i])
        count++;
      else if (a[j] == a[i] && j < i)
        count++;
    }
    temp[count] = a[i];
  }

  #ifndef _OPENMP
  memcpy(a, temp, n*sizeof(int));
  #else

  #pragma omp parallel
  {
    int thread_count = omp_get_num_threads();
    int local_n = n / thread_count;

    #pragma omp parallel for default(none) private(i) shared(n, a, temp, local_n, thread_count)
    for (int i = 0; i < thread_count; i++) {
      int start = i * local_n;
      int nelem = local_n;
      if (i == thread_count - 1) {
        nelem = n - start;
      }
      memcpy(a + start, temp + start, nelem * sizeof(int));
    }
  }

  #endif
  free(temp);
} /* Count sort */


int init_vector(int vect[]) {
    for (int i = 0; i < VECTOR_SIZE; ++i ) {
      vect[i] = rand() % 100;
    }
    return 0;
}


int main(int argc, char** argv) {
  int vector[VECTOR_SIZE];

  init_vector(vector);

  count_sort(vector, VECTOR_SIZE);

  for(int i = 1; i < VECTOR_SIZE; i++) {
    if (vector[i] < vector[i - 1]) {
      return 1;
    }
  }

  return 0;
}
