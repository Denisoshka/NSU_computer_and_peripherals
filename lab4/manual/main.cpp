#include <immintrin.h>
#include <xmmintrin.h>
#include <cfloat>// FLT_MIN
#include <cmath> // fabs()
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
size_t IntrinCount = 128 / 8 / 4;
size_t Alignment = 128 / 8;
size_t N = 4;
size_t M = 2;
void Print(float *matrix);

void GetNorms(float &A_1, float &A_infinity, float *matrix) {
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

inline void FillB(float *matrix, float *B) {
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

void MultMatrix(float *Result, float *Matrix1, float *Matrix2) {
  __m128 *_m128_Result;
  //  __m128 *_m128_Matrix1 = (__m128 *) Matrix1;
  __m128 *_m128_Matrix2;
  _m128_Result = (__m128 *) Result;
  _m128_Matrix2 = (__m128 *) Matrix2;


  int a = sizeof(__m256);
  //  _m128_Rez = _m128_Matrix1
  //  _m128_Matrix2

  memset(Result, 0, N * N * sizeof(*Result));
  //  for (int i = 0; i < N/4; ++i){
  //
  //  }
  for (int i = 0; i < N; ++i) {
    __m128 *_m128_rez;
    _m128_rez = _m128_Result + (i * N / IntrinCount);
    //    float *rez = Result + i * N;
    for (int k = 0; k < N; ++k) {
      //      const float *matr2 = Matrix2 + k * N;
      //      __m256 _m256_matr1 = _m256_Matrix1[i];
      __m128 _m128_tmp;
      __m128 _m128_matr1 = _mm_set1_ps(Matrix1[i * N + k]);
      __m128 *_m128_matr2 = _m128_Matrix2 + (k * N / IntrinCount);
      _m128_matr1 = _mm_set1_ps(Matrix1[i * N + k]);
      _m128_matr2 = _m128_Matrix2 + (k * N / IntrinCount);

      for (int j = 0; j < N / IntrinCount; ++j) {
        _m128_tmp = _mm_mul_ps(_m128_matr1, _m128_matr2[j]);
        _m128_rez[i] = _mm_add_ps(_m128_rez[i], _m128_tmp);
      }
    }
  }
}

inline void SubMatrix(float *Result, float *Reduced, float *Subtrahend) {
  __m128 *_m128_Result = (__m128 *) Result;
  __m128 *_m128_Reduced = (__m128 *) Reduced;
  __m128 *_m128_Subtrahend = (__m128 *) Subtrahend;
  /*
    _m256_Result ;
    _m256_Reduced ;
    _m256_Subtrahend ;
  */

  for (size_t i = 0; i < (N * N) / IntrinCount; ++i) {
    _m128_Result[i] = _mm_sub_ps(_m128_Reduced[i], _m128_Subtrahend[i]);
  }

  /*for (size_t i = 0; i < N * N; ++i) {
    Result[i] = Reduced[i] - Subtrahend[i];
  }*/
}

inline void AddMatrix(float *Result, float *Summand1, float *Summand2) {
  __m128 *_m128_Result = (__m128 *) Result;
  __m128 *_m128_Summand1 = (__m128 *) Summand1;
  __m128 *_m128_Summand2 = (__m128 *) Summand2;

  for (size_t i = 0; i < (N * N) / IntrinCount; ++i) {
    _m128_Result[i] = _mm_add_ps(_m128_Summand1[i], _m128_Summand2[i]);
  }

  /*for (size_t i = 0; i < N * N; ++i) {
    Result[i] = Summand1[i] + Summand2[i];
  }*/
}

void FillR(float *Result, float *I, float *B, float *A) {
  MultMatrix(Result, B, A);
  SubMatrix(Result, I, Result);
}

void InvertMatrix(float *A, float *Result) {
  //  auto *B = new float[N * N];
  //  auto *R = new float[N * N];
  //  auto *I = new float[N * N];
  //  auto Degree1 = new float[N * N];
  //  auto Degree2 = new float[N * N];

  float *R = (float *) std::aligned_alloc(Alignment, N * N * sizeof(float));
  float *B = (float *) std::aligned_alloc(Alignment, N * N * sizeof(float));
  float *I = (float *) std::aligned_alloc(Alignment, N * N * sizeof(float));
  float *Degree1 = (float *) std::aligned_alloc(Alignment, N * N * sizeof(float));
  float *Degree2 = (float *) std::aligned_alloc(Alignment, N * N * sizeof(float));

  FillI(I);
  FillB(A, B);

  FillR(R, I, B, A);

  /*
  bool flag = true;
  AddMatrix(Result, I, R);
  MultMatrix(Degree1, R, R);
  AddMatrix(I, Result, Degree1);

  for (size_t i = 2; i < M; ++i, flag = !flag) {
    MultMatrix((flag) ? Degree2 : Degree1, (flag) ? Degree1 : Degree2, R);
    AddMatrix(I, I, (flag) ? Degree2 : Degree1);
  }
  MultMatrix(Result, I, B);
*/

  free(R);

  free(Degree1);
  free(Degree2);
  free(I);
  free(B);
  /* delete[] Degree1;
   delete[] Degree2;
   delete[] I;
   delete[] R;
   delete[] B;*/
}

void Print(float *matrix) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j)
      std::cout << matrix[N * i + j] << " ";

    std::cout << std::endl;
  }
}

int main() {
  //  auto *A = new float[N * N];
  //  auto *Result = new float[N * N];

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
  float test2[] = {1, 1, 0, 0, 0, 2, 3, 3, 0, 1, 4, 4, 0, 0, 0, 7};
  /*float test2[]
          = {34, 23, 34, 0, 0, 0, 0, 0, 34, 32344, 34, 0, 83, 0, 0, 0, 0, 43, 0, 67,  0, 0,    0, 0, 0,    0, 6968, 0,
             0,  0,  45, 0, 9, 0, 0, 0, 0,  7,     0,  0, 0,  0, 0, 0, 0, 0,  0, 467, 0, 9876, 0, 0, 4548, 0, 68,   0};
  */
  for (int i = 0; i < N * N; ++i) {
    A[i] = test2[i];
  }
  InvertMatrix(A, Result);
  Print(Result);


  //  FillMatrix(A);
  /*InvertMatrix(A, Result);
  Print(Result);
*/
  free(A);
  free(Result);

  //  delete[] A;
  //  delete[] Result;
  return 0;
}
