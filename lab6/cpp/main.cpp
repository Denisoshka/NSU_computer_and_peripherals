#include <fstream>
#include <iostream>

uint64_t L1_CACHE = 1024 * 32;      // 32 KB
uint64_t L2_CACHE = 1024 * 512;     // 512 KB
uint64_t L3_CACHE =  1024 * 1024 * 8;// 8 MB
uint64_t OFFSET = 1024 * 1024 * 16;     // 16 MB

union ticks {
  long long t64;

  struct s32 {
    long th, tl;
  } t32;
} start, end;

void MultiplyMatrices() {
  const int matrixSize = 2048;
  auto M1 = new float[matrixSize * matrixSize];
  auto M2 = new float[matrixSize * matrixSize];
  auto M3 = new float[matrixSize * matrixSize];
  for( size_t i = 0; i < matrixSize; ++i ) {
    float *c = M3 + i * matrixSize;
    for( size_t k = 0; k < matrixSize; ++k ) {
      const float *b = M2 + k * matrixSize;
      float a = M1[i * matrixSize + k];
      for( size_t j = 0; j < matrixSize; ++j ) {
        c[j] += a * b[j];
      }
    }
  }

  for( int i = 0; i < matrixSize * matrixSize; i += matrixSize ) {
    std::cout << M3[i];
  }

  delete[] M1;
  delete[] M2;
  delete[] M3;
}

void fillArray(uint32_t *array, const size_t size, const size_t numberOfFragments,
               const size_t offset) {
  for( size_t i = 0; i < numberOfFragments - 1; i++ ) {
    for( size_t j = 0; j < size; j++ ) {
      array[offset * i + j] = (i + 1) * offset + j;
    }
  }
  for( size_t j = 0; j < size - 1; j++ ) {
    array[offset * (numberOfFragments - 1) + j] = (j + 1);
  }
  array[offset * (numberOfFragments - 1) + size - 1] = 0;
}

double count(uint32_t *arr, size_t size) {
  uint64_t min = UINT64_MAX;
  for( size_t l = 0; l < 20; l++ ) {
    size_t k = 0;
    asm("rdtsc\n" : "=a"(start.t32.th), "=d"(start.t32.tl));
    for( size_t i = 0; i < size; i++ ) {
      k = arr[k];
    }
    asm("rdtsc\n" : "=a"(end.t32.th), "=d"(end.t32.tl));

    if( k == 234 ) {
      std::cout << "lol";
    }
    uint64_t time = end.t64 - start.t64;
    min = (min < time) ? min : time;
  }
  return static_cast<double>(min) / static_cast<double>(size);
}

void warmUpCache(size_t size, size_t numberOfFragments, size_t offset) {
  auto *array = new uint32_t[size];
  fillArray(array, size, numberOfFragments, offset);
  count(array, size);
  delete[] array;
}

int main(int argc, char **argv) {
  size_t n;
  size_t offset = OFFSET / sizeof(uint32_t);
  auto *mas = new uint32_t[OFFSET * 32];
  std::ofstream fOut{"out.txt"};
  MultiplyMatrices();
  for( n = 1; n <= 32; n++ ) {
    size_t size = (L1_CACHE + L2_CACHE + L3_CACHE) / n;
    fillArray(mas, size, n, offset);
    auto x = count(mas, size * n);
    fOut << "fragments " << n << " ticks " << x << std::endl;
  }
  delete[] mas;
  return 0;
}
