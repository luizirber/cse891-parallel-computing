#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef VECTOR_SIZE
#define VECTOR_SIZE 100
#endif


int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}


void qsort_lib(int a[], int n) {

  qsort(a, n, sizeof(int), cmpfunc);

} /* stdlib qsort */


int init_vector(int vect[]) {
    for (int i = 0; i < VECTOR_SIZE; ++i ) {
      vect[i] = rand() % 100;
    }
    return 0;
}


int main(int argc, char** argv) {
  int vector[VECTOR_SIZE];

  init_vector(vector);

  qsort_lib(vector, VECTOR_SIZE);

  for(int i = 1; i < VECTOR_SIZE; i++) {
    if (vector[i] < vector[i - 1]) {
      return 1;
    }
  }

  return 0;
}
