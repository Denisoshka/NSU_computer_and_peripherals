#include <cblas.h>
#include <cfloat>// FLT_MIN
#include <cmath> // fabs()
#include <cstdlib>
#include <iostream>
#include <sys/times.h>// for times
#include <unistd.h>   // for sysconf

size_t Alignment = 256 / 8;
size_t N = 2048;
size_t M = 10;

void GetNorms(float &A_1, float &A_infinity, float *matrix) {
    A_1 = FLT_MIN;
    A_infinity = FLT_MIN;
    float sum_row;
    float sum_column;

    for (int i = 0; i < N; i++) {
        sum_row = 0;
        sum_column = 0;

        for (int j = 0; j < N; j++) {
            sum_row += std::fabs(matrix[N * i + j]);
            sum_column += std::fabs(matrix[j * N + i]);
        }

        A_1 = (sum_row > A_1) ? sum_row : A_1;
        A_infinity = (sum_column > A_infinity) ? sum_column : A_infinity;
    }
}

inline void FillI(float *I) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            I[N * i + j] = (float) (i == j);
        }
    }
}

inline void FillB(float *matrix, float *B) {
    float A_1, A_infinity;
    GetNorms(A_1, A_infinity, matrix);
    float multiplier = A_1 * A_infinity;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            B[N * i + j] = matrix[j * N + i] / multiplier;
        }
    }
}

inline void FillMatrix(float *matrix) {
    for (int i = 0; i < N * N; ++i) {
        matrix[i] = float(random());
        matrix[i] *= (random() % 2) ? 1 : -1;
    }
}

inline void MultMatrix(float *Result, float *Matrix1, float *Matrix2) {
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, Matrix1, N, Matrix2, N, 0.0,
                Result, N);
}

inline void SubMatrix(float *Result, float *Reduced, float *Subtrahend) {
    cblas_saxpy(N * N, -1.0, Subtrahend, 1, Reduced, 1);
    cblas_scopy(N * N, Reduced, 1, Result, 1);
}

inline void AddMatrix(float *Result, float *Summand1, float *Summand2) {
    cblas_saxpy(N * N, 1.0, Summand2, 1, Summand1, 1);
    cblas_scopy(N * N, Summand1, 1, Result, 1);
}

void FillR(float *Result, float *I, float *B, float *A) {
    MultMatrix(Result, B, A);
    SubMatrix(Result, I, Result);
}

void InvertMatrix(float *A, float *Result) {
    float *R = (float *) aligned_alloc(Alignment, N * N * sizeof(float));
    float *B = (float *) aligned_alloc(Alignment, N * N * sizeof(float));
    float *I = (float *) aligned_alloc(Alignment, N * N * sizeof(float));
    float *Degree1 = (float *) aligned_alloc(Alignment, N * N * sizeof(float));
    float *Degree2 = (float *) aligned_alloc(Alignment, N * N * sizeof(float));
    FillI(I);
    FillMatrix(A);
    struct tms start1, end1;
    long clocks_per_sec = sysconf(_SC_CLK_TCK);
    long clocks;
    times(&start1);

    FillB(A, B);
    FillR(R, I, B, A);

    bool flag = true;

    AddMatrix(Result, I, R);
    MultMatrix(Degree1, R, R);
    AddMatrix(I, Result, Degree1);

    for (size_t i = 2; i < M; ++i, flag = !flag) {
        MultMatrix((flag) ? Degree2 : Degree1, (flag) ? Degree1 : Degree2, R);
        AddMatrix(I, I, (flag) ? Degree2 : Degree1);
    }
    MultMatrix(Result, I, B);

    times(&end1);

    clocks = end1.tms_utime - start1.tms_utime;
    std::cout << "Time with BLAS: " <<  (double) clocks / clocks_per_sec <<"sec.\n" << std::endl;

    /// end count time
    free(R);
    free(Degree1);
    free(Degree2);
    free(I);
    free(B);
}

int main() {
    auto A = (float *) std::aligned_alloc(Alignment, N * N * sizeof(float));
    auto Result = (float *) std::aligned_alloc(Alignment, N * N * sizeof(float));

    InvertMatrix(A, Result);
    for (int i = 0; i < 10; i ++){
        std::cout<<Result[i]<<" ";
    }
    free(A);
    free(Result);

    return 0;
}
