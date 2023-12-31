#include <cfloat>// FLT_MIN
#include <cmath> // fabs()
#include <cstring>
#include <ctime>
#include <iostream>
#include <sys/times.h>// for times
#include <unistd.h>   // for sysconf

size_t N = 2048;
size_t M = 10;

void GetNorms(float &A_1, float &A_infinity, const float *matrix) {
    A_1 = FLT_MIN;
    A_infinity = FLT_MIN;
    float sum_row;
    float sum_column;

    for (int i = 0; i < N; i++)// rows
    {
        sum_row = 0;
        sum_column = 0;

        for (int j = 0; j < N; j++)// columns
        {
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

inline void FillB(const float *matrix, float *B) {
    float A_1, A_infinity;
    GetNorms(A_1, A_infinity, matrix);


    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            B[N * i + j] = matrix[j * N + i] / (A_1 * A_infinity);
        }
    }
}


inline void FillMatrix(float *matrix) {
    for (int i = 0; i < N * N; ++i) {
        matrix[i] = float(random());
        matrix[i] *= (random() % 2) ? 1 : -1;
    }
}

void MultMatrix(float *Rez, const float *Matrix1, const float *Matrix2) {
    for (int i = 0; i < N; ++i) {
        float *rez = Rez + i * N;
        memset(rez, 0, N * sizeof(*rez));
        for (int k = 0; k < N; ++k) {
            const float *matr2 = Matrix2 + k * N;
            float matr1 = Matrix1[i * N + k];
            for (int j = 0; j < N; ++j)
                rez[j] += matr1 * matr2[j];
        }
    }
}

void MultMatrix2(float *C, float *A, const float *B) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i * N + j] = 0;
            for (int k = 0; k < N; ++k)
                C[i * N + j] += A[i * N + k] * B[k * N + j];
        }
    }
}


inline void SubMatrix(float *Result, const float *Reduced, const float *Subtrahend) {
    for (size_t i = 0; i < N * N; ++i) {
        Result[i] = Reduced[i] - Subtrahend[i];
    }
}

inline void AddMatrix(float *Result, const float *Summand1, const float *Summand2) {
    for (size_t i = 0; i < N * N; ++i) {
        Result[i] = Summand1[i] + Summand2[i];
    }
}

void FillR(float *Result, const float *I, const float *B, const float *A) {
    MultMatrix(Result, B, A);
    SubMatrix(Result, I, Result);
}

void InvertMatrix(const float *A, float *Result) {
    auto *B = new float[N * N];
    auto *R = new float[N * N];
    auto *I = new float[N * N];
    auto Degree1 = new float[N * N];
    auto Degree2 = new float[N * N];

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
    std::cout << "default matrix mult time taken : " <<  (double) clocks / clocks_per_sec <<"sec.\n" << std::endl;

    delete[] Degree1;
    delete[] Degree2;
    delete[] I;
    delete[] R;
    delete[] B;
}

void Print(const float *matrix) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            std::cout << matrix[N * i + j] << " ";

        std::cout << std::endl;
    }
}

int main() {
    auto *A = new float[N * N];
    auto *Result = new float[N * N];

    FillMatrix(A);
    InvertMatrix(A, Result);

    delete[] A;
    delete[] Result;
    return 0;
}
