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
void run_32_32(int M, int N, int A[N][M], int B[M][N]);
void run_64_64(int M, int N, int A[N][M], int B[M][N]);
void run_other(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]){
    if(M == 32 && N == 32)
        run_32_32(M, N, A, B);
    else if(M == 64 && N == 64)
        run_64_64(M, N, A, B);
    else run_other(M, N, A, B);
}

void run_32_32(int M, int N, int A[N][M], int B[M][N]) {
    int bSize = 8;
    for(int x = 0 ; x < N ; x += bSize) {
        for(int y = 0 ; y < M ; y += bSize) {
            for(int i = 0 ; i < bSize ; i ++) {
                int tem0 = A[x + i][y];
                int tem1 = A[x + i][y + 1];
                int tem2 = A[x + i][y + 2];
                int tem3 = A[x + i][y + 3];
                int tem4 = A[x + i][y + 4];
                int tem5 = A[x + i][y + 5];
                int tem6 = A[x + i][y + 6];
                int tem7 = A[x + i][y + 7];

                B[y][x + i] = tem0;
                B[y + 1][x + i] = tem1;
                B[y + 2][x + i] = tem2;
                B[y + 3][x + i] = tem3;
                B[y + 4][x + i] = tem4;
                B[y + 5][x + i] = tem5;
                B[y + 6][x + i] = tem6;
                B[y + 7][x + i] = tem7;
            }
        }
    }
}

void run_64_64(int M, int N, int A[N][M], int B[M][N]) {
    int a_0, a_1, a_2, a_3, a_4, a_5, a_6, a_7;
    for (int i = 0; i < 64; i += 8){
        for (int j = 0; j < 64; j += 8){
            for (int k = i; k < i + 4; k++){
                // 得到A的第1,2块
                a_0 = A[k][j + 0];
                a_1 = A[k][j + 1];
                a_2 = A[k][j + 2];
                a_3 = A[k][j + 3];
                a_4 = A[k][j + 4];
                a_5 = A[k][j + 5];
                a_6 = A[k][j + 6];
                a_7 = A[k][j + 7];
                // 复制给B的第1,2块
                B[j + 0][k] = a_0;
                B[j + 1][k] = a_1;
                B[j + 2][k] = a_2;
                B[j + 3][k] = a_3;
                B[j + 0][k + 4] = a_4;
                B[j + 1][k + 4] = a_5;
                B[j + 2][k + 4] = a_6;
                B[j + 3][k + 4] = a_7;
            }
            for (int k = j; k < j + 4; k++){
                // 得到B的第2块
                a_0 = B[k][i + 4];
                a_1 = B[k][i + 5];
                a_2 = B[k][i + 6];
                a_3 = B[k][i + 7];
                // 得到A的第3块
                a_4 = A[i + 4][k];
                a_5 = A[i + 5][k];
                a_6 = A[i + 6][k];
                a_7 = A[i + 7][k];
                // 复制给B的第2块
                B[k][i + 4] = a_4;
                B[k][i + 5] = a_5;
                B[k][i + 6] = a_6;
                B[k][i + 7] = a_7;
                // B原来的第2块移动到第3块
                B[k + 4][i + 0] = a_0;
                B[k + 4][i + 1] = a_1;
                B[k + 4][i + 2] = a_2;
                B[k + 4][i + 3] = a_3;
            }
            for (int k = i + 4; k < i + 8; k++)
            {
                // 处理第4块
                a_4 = A[k][j + 4];
                a_5 = A[k][j + 5];
                a_6 = A[k][j + 6];
                a_7 = A[k][j + 7];
                B[j + 4][k] = a_4;
                B[j + 5][k] = a_5;
                B[j + 6][k] = a_6;
                B[j + 7][k] = a_7;
            }
        }
    }
}

void run_other(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, v1, v2, v3, v4, v5, v6, v7, v8;
	int n = N / 8 * 8;
	int m = M / 8 * 8;
	for (j = 0; j < m; j += 8) {
		for (i = 0; i < n; ++i) {
			v1 = A[i][j];
			v2 = A[i][j+1];
			v3 = A[i][j+2];
			v4 = A[i][j+3];
			v5 = A[i][j+4];
			v6 = A[i][j+5];
			v7 = A[i][j+6];
			v8 = A[i][j+7];
			
			B[j][i] = v1;
			B[j+1][i] = v2;
			B[j+2][i] = v3;
			B[j+3][i] = v4;
			B[j+4][i] = v5;
			B[j+5][i] = v6;
			B[j+6][i] = v7;
			B[j+7][i] = v8;
		}
    }
	for (i = 0; i < N; ++i) {
		for (j = m; j < M; ++j) {
			v1 = A[i][j];
			B[j][i] = v1;
		}
    }
	for (i = n; i < N; ++i) {
		for (j = 0; j < M; ++j) {
			v1 = A[i][j];
			B[j][i] = v1;
		}
    }
}


/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char first_try_desc[] = "first_try";
void first_try(int M, int N, int A[N][M], int B[M][N]){
    int bSize = 8;
    for(int x = 0 ; x < N ; x += bSize) {
        for(int y = 0 ; y < M ; y += bSize) {
            for(int i = 0 ; i < bSize && x + i < M; i ++) {
                for(int j = 0 ; j < bSize && y + j < N; j ++) {
                    B[y + j][x + i] = A[x + i][y + j];
                }
            }
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
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    //registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
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

