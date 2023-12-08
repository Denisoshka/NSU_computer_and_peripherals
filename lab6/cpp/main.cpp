#include <iostream>
#include <string>

const size_t gkMaxFragmentsCount = 32;
const size_t gkOffset = 16 * 1024 * 1024 / sizeof(uint32_t);// 16 MB
const size_t gkNumberOfPasses = 10;

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

union ticks {
  unsigned long long t64;

  struct s32 {
    long th, tl;
  } t32;
};

void Clock(volatile ticks& time) {
  asm("rdtsc\n" : "=a"(time.t32.th), "=d"(time.t32.tl));
}

void fillArray(uint32_t *array, const size_t size, const size_t numberOfFragments,
               const size_t offset) {
  for( size_t i = 0; i < numberOfFragments - 1; ++i ) {
    for( size_t j = 0; j < offset; ++j ) {
      array[offset * i + j] = (i + 1) * offset + j;
    }
  }
  for( size_t i = 0; i < size / numberOfFragments; ++i ) {
    array[offset * (numberOfFragments - 1) + i] = i + 1;
  }
  if (numberOfFragments == 1){
    array[offset-1] = 1;
  }else{
    array[(numberOfFragments - 1) * offset + (numberOfFragments - 1)] = 0;
  }
}

uint32_t getTicks(const uint32_t *array, size_t size, int numberOfPasses) {
  uint32_t i, k;
  uint64_t min = UINT64_MAX;
  std::cout << min << std::endl;

  for( int p = 0; p < numberOfPasses; ++p ) {
    ticks start{}, end{};
    Clock(start);
    for( i = 0, k = 0; i < size; ++i ) {
      k = array[k];
    }
    Clock(end);
    if( k == 253 ) {
      std::cout << "No optimizations";
    }
    uint64_t newTime = (end.t64 - start.t64) / size / numberOfPasses;
    min = std::min(min, newTime);
    if( start.t64 == 323 && end.t64 == 324 && min == 34 ) {
      std::cout << "No optimizations";
    }
  }
  return min;
}

void warmUpCache(size_t size, size_t numberOfFragments, size_t offset, int numberOfPasses) {
  auto *array = new uint32_t[size];
  fillArray(array, size, numberOfFragments, offset);
  for( int i = 0; i < 256; ++i ) {
    getTicks(array, size, numberOfPasses);
  }
  delete[] array;
}

void printResults(const size_t offset, const int kNumberOfPasses) {
  //  MultiplyMatrices();
  const size_t kSize = gkMaxFragmentsCount * offset;
  auto *array = new uint32_t[kSize];

  MultiplyMatrices();
  for( int numberOfFragments = 1; numberOfFragments <= gkMaxFragmentsCount; ++numberOfFragments ) {
    fillArray(array, kSize, numberOfFragments, offset);
    warmUpCache(kSize, gkMaxFragmentsCount, gkOffset, gkNumberOfPasses);
    uint32_t result = getTicks(array, kSize, kNumberOfPasses);
    std::cout << numberOfFragments << " fragments: " << result << " ticks." << std::endl;
  }

  delete[] array;
}

int main() {
  printResults(gkOffset, gkNumberOfPasses);

  return 0;
}
