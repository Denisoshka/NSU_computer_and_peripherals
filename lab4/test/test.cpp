#include <cfloat>       // FLT_MIN
#include <cmath>        // fabs()
#include <ctime>
#include <iostream>

#define N 3
#define M 10

void Inverse(const float * matrix,
             float * result);
void GetNorms(float & A_1,
              float & A_infinity,
              const float * matrix);
void FillB(const float * matrix,
           float * B);
void FillI(float * I);
void Multiplication(const float * multiplier1,
                    const float * multiplier2,
                    float * result);
void Addition(const float * addend1,
              const float * addend2,
              float * result);
void Subtraction(const float * minuend,
                 const float * subtrahend,
                 float * result);
void Copy(float * dest,
          const float * src);
void Print(const float * matrix);

int main()
{
  srandom(time(nullptr));
  auto * matrix = new float [N * N];
  auto * result = new float [N * N];
  auto * check = new float [N * N];
  timespec start = {
          0,
          0
  };
  timespec end = {
          0,
          0
  };

  for (int i = 0; i < N * N; ++i)
  {
    matrix[i] = float(random());
    matrix[i] *= (random() % 2) ? 1 : -1;
    result[i] = 0;
  }
//
//  float test[] = {1, 0, 0, 0, 2, 3, 0, 0, 4};
//  for (int i = 0; i < N * N; ++i) {
//    matrix[i] = test[i];
//  }

  clock_gettime(CLOCK_MONOTONIC_RAW,
                &start);
  Inverse(matrix,
          result);
//  Print(result);
  clock_gettime(CLOCK_MONOTONIC_RAW,
                &end);

  Multiplication(matrix,
                 result,
                 check);

  float A_1, A_infinity;
  GetNorms(A_1,
           A_infinity,
           check);

  std::cout << "A_1 check: " << A_1 << std::endl;
  std::cout << "A_infinity check: " << A_infinity << std::endl;
  Print(check);

  std::cout << "Time without vectorization: "
            << (double)end.tv_sec - (double)start.tv_sec + 1e-9 * ((double)end.tv_nsec - (double)start.tv_nsec)
            << " sec." << std::endl;

  delete []matrix;
  delete []result;
  delete []check;

  return EXIT_SUCCESS;
}

void Inverse(const float * matrix,
             float * result)
{
  auto * B = new float[N * N];
  auto * I = new float[N * N];
  auto * tmp = new float[N * N];
  auto * R = new float[N * N];
  bool flag = true;

  FillB(matrix,
        B);
  FillI(I);
  Multiplication(B,
                 matrix,
                 tmp);
  Subtraction(I,
              tmp,
              R);
  Addition(I,
           R,
           tmp);
  Copy(result,
       R);

  for (int i = 2; i < M; ++i)
  {
    Multiplication(flag ? result : I,
                   R,
                   flag ? I : result);
    Addition(tmp,
             flag ? I : result,
             tmp);
    flag = !flag;
  }

  Multiplication(tmp,
                 B,
                 result);

  delete[] I;
  delete[] B;
  delete[] tmp;
  delete[] R;
}

void GetNorms(float & A_1,
              float & A_infinity,
              const float * matrix)
{
  A_1 = FLT_MIN;
  A_infinity = FLT_MIN;
  float sum_row = 0;
  float sum_column = 0;

  for (int i = 0; i < N; i++) // rows
  {
    sum_row = 0;
    sum_column = 0;

    for (int j = 0; j < N; j++) // columns
    {
      sum_row += std::fabs(matrix[N * i + j]);
      sum_column += std::fabs(matrix[j * N + i]);
    }

    if (sum_row > A_1)
      A_1 = sum_row;
    if (sum_column > A_infinity)
      A_infinity = sum_column;
  }
}

void FillB(const float * matrix,
           float * B)
{
  float A_1, A_infinity;
  GetNorms(A_1,
           A_infinity,
           matrix);

  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      B[N * i + j] = matrix[j * N + i] / (A_1 * A_infinity);
}

void FillI(float * I)
{
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      I[N * i + j] = (float)(i == j);
}

void Multiplication(const float * multiplier1,
                    const float * multiplier2,
                    float * result)
{
  for (int i = 0; i < N * N; ++i)
    result[i] = 0;

  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      for (int k = 0; k < N; ++k)
        result[N * i + k] += multiplier1[N * i + j] * multiplier2[N * j + k];
}

void Addition(const float * addend1,
              const float * addend2,
              float * result)
{
  for (int i = 0; i < N * N; ++i)
    result[i] = addend1[i] + addend2[i];
}

void Subtraction(const float * minuend,
                 const float * subtrahend,
                 float * result)
{
  for (int i = 0; i < N * N; i++)
    result[i] = minuend[i] - subtrahend[i];
}

void Copy(float * dest,
          const float * src)
{
  for (int i = 0; i < N * N; i++)
    dest[i] = src[i];
}

void Print(const float * matrix)
{
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
      std::cout << matrix[N * i + j] << " ";

    std::cout << std::endl;
  }
}