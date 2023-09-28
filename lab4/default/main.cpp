#include <cfloat>// FLT_MIN
#include <cmath> // fabs()
#include <cstring>
#include <ctime>
#include <iostream>

size_t N = 5;
size_t M = 10000000;
void Print(const float *matrix);

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
  /*
    for (int i = 0; i < N * N; ++i) {
      I[i] = (float) (i / N == i % N);
    }*/
}

inline void FillB(const float *matrix, float *B) {
  float A_1, A_infinity;
  GetNorms(A_1, A_infinity, matrix);


  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      B[N * i + j] = matrix[j * N + i] / (A_1 * A_infinity);
    }
  }

  /*
    for (int i = 0; i < N * N; ++i) {
      B[i] = matrix[(i % N) * N + i / N] / (A_1 * A_infinity);
    }*/
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
  float test2[] = {1, 2, 4, 3, 5, 7, 6, 8, 9};
  for (int i = 0; i < N * N; ++i) {
    A[i] = test2[i];
  }
  InvertMatrix(A, Result);
  Print(Result);
  */

//  FillMatrix(A);
  /*float test2[] = {34, 23, 34, 0, 0, 0, 0, 0,
                   34, 32344, 34, 0, 83, 0, 0, 0,
                   0, 43, 0, 67, 0, 0,  0,  0,
                   0, 0, 6968, 0, 0, 0,  45, 0,
                   9, 0, 0, 0, 0, 7, 0,  0,
                   0, 0, 0, 0, 0, 0, 0, 467,
                   0, 9876, 0, 0, 4548, 0, 68, 0
  };*/
  /*float test2[] = {1,1,0,0,
                   0,2,3,3,
                   0,1,4,4,
                   0,0,0,7};*/
/*  float test2[] = {1,1,0,0,4,
                   0,2,3,3,5,
                   0,1,4,4,7,
                   0,0,0,7,0,
                   0,3,0,0,1};*/
  for (int i = 0; i < N * N; ++i) {
    A[i] = test2[i];
  }

  InvertMatrix(A, Result);
  Print(Result);

  delete[] A;
  delete[] Result;
  return 0;
}
