/* Mahmoud Elnagar - 28/12/2022 */
/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N]){
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */

void copy_row(int BS, int src[], int dst[]){
    int a0, a1, a2, a3, a4, a5, a6, a7;

    if(BS > 0) a0 = src[0];
    if(BS > 1) a1 = src[1];
    if(BS > 2) a2 = src[2];
    if(BS > 3) a3 = src[3];
    if(BS > 4) a4 = src[4];
    if(BS > 5) a5 = src[5];
    if(BS > 6) a6 = src[6];
    if(BS > 7) a7 = src[7];

    if(BS > 0) dst[0] = a0;
    if(BS > 1) dst[1] = a1;
    if(BS > 2) dst[2] = a2;
    if(BS > 3) dst[3] = a3;
    if(BS > 4) dst[4] = a4;
    if(BS > 5) dst[5] = a5;
    if(BS > 6) dst[6] = a6;
    if(BS > 7) dst[7] = a7;
}

void copy_block(int BS, int N, int* src, int*dst){
  for(int i = 0; i < BS; ++i){
    copy_row(BS, &src[i*N], &dst[i*N]);
  }
}

void mirror_block(int BS, int N, int* B){
  for(int i = 0; i < BS; ++i){
    for(int j = i + 1; j < BS; ++j){
      int temp = B[i*N +j];
      B[i*N +j] = B[j*N +i];
      B[j*N +i] = temp;
    }
  }
}

void transpose_regular(int N, int A[N][N], int B[N][N]){
    const int BS = (N == 32)? 8 : 4;
    for(int i = 0; i < N; i +=BS){
        for(int j = 0; j < N; j+= BS){
            copy_block(BS, N, &A[j][i], &B[i][j]);
            mirror_block(BS, N, &B[i][j]);
        }
    }
}

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]){
    if (N == M) return transpose_regular(N, A, B);
    trans(M,N,A,B);
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions(){
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]){
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

