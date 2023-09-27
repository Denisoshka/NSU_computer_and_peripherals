#include <cfloat>// FLT_MIN
#include <cmath> // fabs()
#include <ctime>
#include <iostream>

size_t N = 48;
size_t M = 10;


void GetNorms(float &A_1, float &A_infinity, const float *matrix) {
  A_1 = FLT_MIN;
  A_infinity = FLT_MIN;
  float sum_row = 0;
  float sum_column = 0;

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
  /*for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      I[N * i + j] = (float) (i == j);*/

  for (int i = 0; i < N * N; ++i) {
    I[i] = (float) (i / N == i % N);
  }
}

inline void FillB(const float *matrix, float *B) {
  float A_1, A_infinity;
  GetNorms(A_1, A_infinity, matrix);

  /*for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      B[N * i + j] = matrix[j * N + i] / (A_1 * A_infinity);*/

  for (int i = 0; i < N * N; ++i) {
    B[i] = matrix[(i % N) * N + i / N] / (A_1 * A_infinity);
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
    for (int j = 0; j < N; ++j)
      rez[j] = 0;
    for (int k = 0; k < N; ++k) {
      const float *matr2 = Matrix2 + k * N;
      float matr1 = Matrix1[i * N + k];
      for (int j = 0; j < N; ++j)
        rez[j] += matr1 * matr2[j];
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
    Result[i] = Summand1[i] - Summand2[i];
  }
}

void InvertMatrix(const float *A, float *B, float *I, float *R, float *Result) {
  FillB(A, B);
  MultMatrix(R, A, B);
  SubMatrix(R, I, R);

  AddMatrix(Result, I, R);

  auto RDegree = B;
  MultMatrix(RDegree, R, R);
  AddMatrix(Result, Result, R);

  for (size_t i = 0; i < M - 2; ++i) {
    MultMatrix(RDegree, RDegree, R);
    AddMatrix(Result, Result, RDegree);
  }
}

void Print(const float *matrix) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j)
      std::cout << matrix[N * i + j] << " ";

    std::cout << std::endl;
  }
}

int main() {

  float *A = nullptr;
  float *B = nullptr;
  float *R = nullptr;
  float *I = nullptr;
  float *Result = nullptr;

  try {
    A = new float[N];
  } catch (...) {
    return 1;
  }
  try {
    B = new float[N];
  } catch (...) {
    return 2;
  }
  try {
    R = new float[N];
  } catch (...) {
    return 3;
  }
  try {
    I = new float[N];
  } catch (...) {
    return 4;
  }
  try {
    Result = new float[N];
  } catch (...) {
    return 5;
  }

  FillMatrix(A);
  FillI(I);
  FillB(A, B);

  InvertMatrix(A, B, I, R, Result);

  Print(Result);
  delete[] A;
  delete[] B;
  delete[] I;
  delete[] R;
  delete[] Result;
  return 0;
}
