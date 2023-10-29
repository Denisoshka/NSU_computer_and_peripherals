#include <immintrin.h>
#include <cfloat>// FLT_MIN
#include <cmath> // fabs()
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

size_t IntrinCount = 256 / 8 / 4;
size_t Alignment = 256 / 8;
size_t N = 2048;
size_t M = 10;

void Print(float *matrix);

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
    /*
      for (int i = 0; i < N * N; ++i) {
        I[i] = (float) (i / N == i % N);
      }*/
}

inline void FillB(float *matrix, float *B) {
    float A_1, A_infinity;
    GetNorms(A_1, A_infinity, matrix);
    float multiplier = (A_1 * A_infinity);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            B[N * i + j] = matrix[j * N + i];
        }
    }
    __m256 * _m256_B = (__m256 * )B;
    __m256 _m256_multiplier = _mm256_set1_ps(multiplier);
    for (int i = 0; i < N*N/IntrinCount; ++i){
      _m256_B[i] =   _mm256_div_ps(_m256_B[i], _m256_multiplier);
    }
}


inline void FillMatrix(float *matrix) {
    for (int i = 0; i < N * N; ++i) {
        matrix[i] = float(random());
        matrix[i] *= (random() % 2) ? 1 : -1;
    }
}

void MultMatrix(float *Result, float *Matrix1, float *Matrix2) {
    __m256 *_m256_Result;
    __m256 *_m256_Matrix2;
    _m256_Result = (__m256 *) Result;
    _m256_Matrix2 = (__m256 *) Matrix2;

    memset(Result, 0, N * N * sizeof(*Result));

    for (int i = 0; i < N; ++i) {
        __m256 *_m256_rez = _m256_Result + (i * N / IntrinCount);

        for (int k = 0; k < N; ++k) {
            __m256 _m256_tmp;
            __m256 _m256_matr1 = _mm256_set1_ps(Matrix1[i * N + k]);
            __m256 *_m256_matr2 = _m256_Matrix2 + (k * N / IntrinCount);

            for (int j = 0; j < N / IntrinCount; ++j) {
                _m256_tmp = _mm256_mul_ps(_m256_matr1, _m256_matr2[j]);
                _m256_rez[j] = _mm256_add_ps(_m256_rez[j], _m256_tmp);
            }
        }
    }
}

inline void SubMatrix(float *Result, float *Reduced, float *Subtrahend) {
    __m256 *_m256_Result = (__m256 *) Result;
    __m256 *_m256_Reduced = (__m256 *) Reduced;
    __m256 *_m256_Subtrahend = (__m256 *) Subtrahend;

    for (size_t i = 0; i < (N * N) / IntrinCount; ++i) {
        _m256_Result[i] = _mm256_sub_ps(_m256_Reduced[i], _m256_Subtrahend[i]);
    }
}

inline void AddMatrix(float *Result, float *Summand1, float *Summand2) {
    __m256 *_m256_Result = (__m256 *) Result;
    __m256 *_m256_Summand1 = (__m256 *) Summand1;
    __m256 *_m256_Summand2 = (__m256 *) Summand2;

    for (size_t i = 0; i < (N * N) / IntrinCount; ++i) {
        _m256_Result[i] = _mm256_add_ps(_m256_Summand1[i], _m256_Summand2[i]);
    }
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
//  start count time //////////////////////////////
    FillMatrix(A);
    timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW,
                  &start);

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

    clock_gettime(CLOCK_MONOTONIC_RAW,
                  &end);
    std::cout << "Time with vectorization: "
              << (double) end.tv_sec - (double) start.tv_sec + 1e-9 * ((double) end.tv_nsec - (double) start.tv_nsec)
              << " sec." << std::endl;
///////////////////////////////////////
    free(R);
    free(Degree1);
    free(Degree2);
    free(I);
    free(B);
}


void Print(float *matrix) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << matrix[N * i + j] << " ";
        }
        std::cout << std::endl;
    }
}


int main() {
    auto A = (float *) std::aligned_alloc(Alignment, N * N * sizeof(float));
    auto Result = (float *) std::aligned_alloc(Alignment, N * N * sizeof(float));

    /*
    float test1[] = {1, 1, 0, 0, 2, 3, 0, 1, 4};
    for (int i = 0; i < N * N; ++i) {
      A[i] = test1[i];
    }
    Print(A);
    std::cout << "====================================================================" << std::endl;
    InvertMatrix(A, Result);
    Print(Result);
    std::cout << "====================================================================" << std::endl;
    */
    float test2[] = {9, 1, 0, 0, 5, 2, 3, 3, 0, 1, 4, 4, 0, 0, 0, 9};
    /*float test2[]
            = {34, 23, 34, 0, 0, 0, 0, 0, 34, 32344, 34, 0, 83, 0, 0, 0, 0, 43, 0, 67,  0, 0,    0, 0, 0,    0, 6968, 0,
               0,  0,  45, 0, 9, 0, 0, 0, 0,  7,     0,  0, 0,  0, 0, 0, 0, 0,  0, 467, 0, 9876, 0, 0, 4548, 0, 68,   0};
    */
    /*for (int i = 0; i < N * N; ++i) {
      A[i] = test2[i];
    }*/


    InvertMatrix(A, Result);

//  Print(Result);

    free(A);
    free(Result);

    return 0;
}
