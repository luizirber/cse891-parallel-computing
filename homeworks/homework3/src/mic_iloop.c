#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef VECTOR_SIZE
#define VECTOR_SIZE 100
#endif

void count_sort(int a[], int n) {
  int i, j, count;
  int *temp = malloc(n*sizeof(int));

  #pragma offload target(mic) in(a:length(n)) out(temp:length(n))
  {
  #pragma omp parallel for default(none) private(i, j, count) shared(a, temp, n)
  for (i = 0; i < n; i++) {
    count = 0;
    for (j = 0; j < n; j++) {
      if (a[j] < a[i])
        count++;
      else if (a[j] == a[i] && j < i)
        count++;
    }
    temp[count] = a[i];
  }
  }

  memcpy(a, temp, n*sizeof(int));
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

  return 0;
}
