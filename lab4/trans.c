/*
 * CSE 351 Lab 4 (Caches and Cache-Friendly Code)
 * Part 2 - Optimizing Matrix Transpose
 *
 * Name(s): Tatsuhiko Araki 
 * NetID(s): taraki
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[M][N], int B[N][M]);
 * and compute B = A^T.
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1 KiB direct mapped cache with a block size of 32 bytes.
 */

#include <stdio.h>
#include "support/cachelab.h"

int is_transpose(int M, int N, int A[M][N], int B[N][M]);


/*
 * transpose_submit - This is the transpose function that you will be graded
 *     on. Do not change the description string "Transpose submission", as the
 *     driver searches for that string to identify the transpose function to be
 *     graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[M][N], int B[N][M]) {
	if ( M == 32 ) {
		int tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
		for ( int i = 0; i < M; i += 8 ) {
			for ( int j = 0; j < N; j += 8 ) {
				for ( int k = i; k < i + 8; k++ ) {
					tmp1 = A[k][j];
					tmp2 = A[k][j + 1];
					tmp3 = A[k][j + 2];
					tmp4 = A[k][j + 3];
					tmp5 = A[k][j + 4];
					tmp6 = A[k][j + 5];
					tmp7 = A[k][j + 6];
					tmp8 = A[k][j + 7];
					B[j][k] = tmp1;
					B[j + 1][k] = tmp2;
					B[j + 2][k] = tmp3;
					B[j + 3][k] = tmp4;
					B[j + 4][k] = tmp5;
					B[j + 5][k] = tmp6;
					B[j + 6][k] = tmp7;
					B[j + 7][k] = tmp8;
				}	
			}
		}
	}
	if ( M == 64 ) {
		int tmp1, tmp2, tmp3, tmp4;
               	for ( int i = 0; i < M; i += 4 ) {
                       	for ( int j = 0; j < N; j += 4 ) {
                               	for ( int k = i; k < i + 4; k++ ) {
                                        tmp1 = A[k][j];
                                       	tmp2 = A[k][j + 1];
                                       	tmp3 = A[k][j + 2];
                                       	tmp4 = A[k][j + 3];	
                                        B[j][k] = tmp1;
                                        B[j + 1][k] = tmp2;
                                        B[j + 2][k] = tmp3;
                                        B[j + 3][k] = tmp4;
                                }
                        }
                }
	}
}


// You can define additional transpose functions below. We've defined a simple
// one below to help you get started.

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[M][N], int B[N][M]) {
    int i, j, tmp;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}


/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[M][N], int B[N][M]) {
    int i, j;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
