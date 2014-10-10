#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#ifndef VECTOR_SIZE
#define VECTOR_SIZE 6144
#endif

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 16
#endif


int init_matrix(double M[][VECTOR_SIZE]) {
  for (int i = 0; i < VECTOR_SIZE; ++i ) {
    for (int j = 0; j < VECTOR_SIZE; ++j ) {
      M[i][j] = drand48() * 100;
      //M[i][j] = i;
    }
  }
  return 0;
}

void naive_multiplication(
    double A[][VECTOR_SIZE],
    double B[][VECTOR_SIZE],
    double C[][VECTOR_SIZE]) {
  for (int i = 0; i < VECTOR_SIZE; ++i ) {
    for (int j = 0; j < VECTOR_SIZE; ++j ) {
      for (int k = 0; k < VECTOR_SIZE; ++k ) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

void blocked_multiplication(
    double A[][VECTOR_SIZE],
    double B[][VECTOR_SIZE],
    double C[][VECTOR_SIZE]) {

  int i, j, k, kk, jj;
  double sum;
  const int NUMBER_OF_BLOCKS = BLOCK_SIZE * (VECTOR_SIZE / BLOCK_SIZE);

  for (kk = 0; kk < NUMBER_OF_BLOCKS; kk += BLOCK_SIZE) {
    for (jj = 0; jj < NUMBER_OF_BLOCKS; jj += BLOCK_SIZE) {
      for (i = 0; i < VECTOR_SIZE ; i++) {
        for (j = jj; j < jj + BLOCK_SIZE; j++) {
          for (k = kk; k < kk + BLOCK_SIZE; k++) {
            C[i][j] += A[i][k] * B[k][j];
          }
        }
      }
    }
  }
}


int main(int argc, char **argv) {
  srand48(time(NULL));

  double A[VECTOR_SIZE][VECTOR_SIZE];
  double B[VECTOR_SIZE][VECTOR_SIZE];
  double C[VECTOR_SIZE][VECTOR_SIZE];

  init_matrix(A);
  init_matrix(B);
  memset(C, 0, VECTOR_SIZE * VECTOR_SIZE * sizeof(double));

  blocked_multiplication(A, B, C);

  double D[VECTOR_SIZE][VECTOR_SIZE];
  memset(D, 0, VECTOR_SIZE * VECTOR_SIZE * sizeof(double));
  naive_multiplication(A, B, D);
  int equal = 1;
  for (int i = 0; i < VECTOR_SIZE; ++i ) {
    for (int j = 0; j < VECTOR_SIZE; ++j ) {
      if (C[i][j] != D[i][j])
          equal = 0;
    }
  }

  if (equal) {
    printf("both are equal\n");
  } else {
    printf("they are different\n");
  }

  /*
  for (int i = 0; i < VECTOR_SIZE; ++i ) {
    for (int j = 0; j < VECTOR_SIZE; ++j ) {
      printf("%d ", C[i][j]);
    }
    printf("\n");
  }
  */

  return(0);
}
