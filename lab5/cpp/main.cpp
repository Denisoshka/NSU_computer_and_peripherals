#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

enum MODE {
  DIRECT,
  REVERSE,
  RANDOM
};

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

void directTraversal(uint32_t *array, const uint32_t n) {
  array[n - 1] = 0;
  for( uint32_t i = 0; i < n - 1; i++ ) {
    array[i] = i + 1;
  }
}

void reverseTraversal(uint32_t *array, const uint32_t n) {
  array[0] = n - 1;
  for( uint32_t i = n - 1; i > 0; --i )
    array[i] = i - 1;
}

void randomTraversal(uint32_t *array, const uint32_t arraySize) {
  std::set<uint32_t> set;

  for( uint32_t i = 0; i < arraySize; ++i )
    set.insert(i);
  uint32_t index = rand() % set.size();
  uint32_t prev = *next(set.begin(), index);
  uint32_t start = prev;

  for( ; set.size() > 1; ) {
    set.erase(next(set.begin(), index));
    index = rand() % set.size();
    array[prev] = *next(set.begin(), index);
    prev = *next(set.begin(), index);
  }

  array[*set.begin()] = start;
}

void myClock(uint64_t& time) {
  uint64_t s;
  asm("rdtsc\n" : "=a"(s));
  time = s;
  //  asm("rdtsc\n" : "=a"(time));
}

void assign(const uint32_t *array, const uint32_t n, uint64_t& timeStart, uint64_t& timeEnd,
            const uint32_t k = 1) {
  uint32_t m = 0;

  myClock(timeStart);
  for( uint32_t i = 0; i < n * k; ++i ) {
    m = array[m];
  }
  myClock(timeEnd);

  if( m == 22423456 ) {
    std::cout << "lol";
  }
}

void CalcTime(const std::string& streamOutName, MODE modeName, const uint32_t minSize,
              const uint32_t maxSize, uint32_t k = 1) {

  std::ofstream streamOut{streamOutName};
  uint32_t currentSize = minSize;

  MultiplyMatrices();

  for( ; currentSize < maxSize; currentSize = (uint32_t)(currentSize * 1.2) ) {
    auto *array = new uint32_t[currentSize];

    if( modeName == DIRECT )
      directTraversal(array, currentSize);
    else if( modeName == REVERSE )
      reverseTraversal(array, currentSize);
    else
      randomTraversal(array, currentSize);

    uint64_t minTime = LLONG_MAX;
    uint64_t st, nd;
    assign(array, currentSize, st, nd, 3);

    for( int i = 0; i < 3; ++i ) {
      uint64_t timeStart, timeEnd;
      assign(array, currentSize, timeStart, timeEnd, k);
      minTime = std::min(timeEnd - timeStart, minTime);
    }

    streamOut << currentSize << " " << minTime / (k * currentSize) << std::endl;

    delete[] array;
  }
}

int main() {
  uint32_t minSize = 1024 / sizeof(minSize);
  uint32_t maxSize = 16777216 / sizeof(maxSize);
  uint32_t k = 5;

  std::ofstream streamOut{};

  CalcTime("direct.txt", DIRECT, minSize, maxSize,  k);

  CalcTime("reverse.txt", REVERSE, minSize, maxSize, k);

  CalcTime("random.txt", RANDOM, minSize, maxSize,  k);
}
