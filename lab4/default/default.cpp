#include <iostream>
#include <cmath>
#include <ctime>
#include <limits>
#include <sys/times.h>// for times
#include <unistd.h>   // for sysconf


const int N = 2048;
const int M = 10;

///////////////////////////////////////////////////////////////////////

float GetMultMaxSums(float *matrix);

void DefineB(float *matrix, float *B);

void DefineI(float *I);

void DefineTmp(float *tmp);

void Mult(float *matrix1, float *matrix2, float *result);

void Additional(float *matrix1, float *matrix2, float *result);

void Subtraction(float *matrix1, float *matrix2, float *result);

void Inverse(float *matrix, float *result);

void PrintMatrix(const float *matrix);

void CopyMatrix(float *matrixInput, float *matrixOutput);

//////////////////////////////////////////////////////////////////////

int main() {
    srand(time(0));

    auto *matrix = new float[N * N];
    auto *result = new float[N * N];

    for (int i = 0; i < N * N; ++i) {
        matrix[i] = rand() % 10;
        result[i] = 0;
    }

    // PrintMatrix(matrix);
    struct tms start1, end1;
    long clocks_per_sec = sysconf(_SC_CLK_TCK);
    long clocks;
    times(&start1);
    Inverse(matrix, result);
    times(&end1);
    // PrintMatrix(result);
    clocks = end1.tms_utime - start1.tms_utime;
    std::cout << "default matrix mult time taken: " <<  (double) clocks / clocks_per_sec <<"sec.\n" << std::endl;
    return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

float GetMultMaxSums(float *matrix) {
    float currentSumRow = 0;
    float currentSumColumn = 0;

    float maxSumRow = std::numeric_limits<float>::min();
    float maxSumColumn = std::numeric_limits<float>::min();

    for (int i = 0; i < N; ++i) {
        currentSumColumn = 0;
        currentSumRow = 0;

        for (int j = 0; j < N; ++j) {
            currentSumRow += matrix[N * i + j];
            currentSumColumn += matrix[j * N + i];
        }

        maxSumRow = std::max(currentSumRow, maxSumRow);
        maxSumColumn = std::max(currentSumColumn, maxSumColumn);
    }

    return maxSumColumn * maxSumRow;
}

void DefineB(float *matrix, float *B) {
    float divider = GetMultMaxSums(matrix);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            B[N * i + j] = matrix[j * N + i] / divider;
}

void DefineI(float *I) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            (i == j) ? I[N * i + j] = 1 : I[N * i + j] = 0;
}

void DefineTmp(float *tmp) {
    for (int i = 0; i < N * N; ++i) {
        tmp[i] = 0;
    }
}

void Mult(float *matrix1, float *matrix2, float *result) {
  /*  memset(result, 0, N * N * sizeof(*result));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k)
                result[i * N + j] += matrix1[i * N + k] * matrix2[k * N + j];
        }
    }*/
    for (int i = 0; i < N; ++i) {
        float *rez = result + i * N;
        for (int k = 0; k < N; ++k) {
            const float *matr2 = matrix2 + k * N;
            float matr1 = matrix1[i * N + k];
            for (int j = 0; j < N; ++j)
                rez[j] += matr1 * matr2[j];
        }
    }
}

void Subtraction(float *matrix1, float *matrix2, float *result) {
    for (int i = 0; i < N * N; i++)
        result[i] = matrix1[i] - matrix2[i];
}

void Additional(float *matrix1, float *matrix2, float *result) {
    for (int i = 0; i < N * N; i++)
        result[i] = matrix1[i] + matrix2[i];
}

void Inverse(float *matrix, float *result) {
    auto *R = new float[N * N];
    auto *B = new float[N * N];
    auto *I = new float[N * N];

    auto *tmp = new float[N * N];
    DefineTmp(tmp);
    DefineB(matrix, B);
    DefineI(I);

    Mult(B, matrix, tmp);   // tmp = BA
    Subtraction(I, tmp, R); // R = I - BA
    Additional(I, R, tmp);  // tmp = I + R
    CopyMatrix(R, result);  // result = R

    for (int i = 2; i < M; ++i) {
        if (i % 2 == 0) {
            Mult(R, result, I);    // I = result * R
            Additional(tmp, I, tmp); // tmp = tmp + result
        } else {
            Mult(R, I, result);    // result = I * R
            Additional(tmp, result, tmp); // tmp = tmp + I
        }
    }

    Mult(tmp, B, result); // result = tmp * B

    delete[] I;
    delete[] R;
    delete[] B;
    delete[] tmp;
}

void PrintMatrix(const float *matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            std::cout << matrix[N * i + j] << " ";

        std::cout << std::endl;
    }
}

void CopyMatrix(float *matrixInput, float *matrixOutput) {
    for (int i = 0; i < N * N; i++)
        matrixOutput[i] = matrixInput[i];
}
