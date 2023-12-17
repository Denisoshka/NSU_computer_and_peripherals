/*
#include <iostream>
#include <string>

const size_t L1_CACHE = 32 * 1024;                        // 32 KB
const size_t L2_CACHE = 512 * 1024;                       // 512 KB
const size_t L3_CACHE = 4 * 1024 * 1024;                  // 4 MB
const size_t OFFSET = 16 * 1024 * 1024 / sizeof(uint32_t);// 16 MB

void fillArray(int *array, size_t size, size_t numberOfFragments, size_t offset) {
  for( size_t i = 0; i < numberOfFragments; ++i ) {
    for( size_t j = 0; j < size / numberOfFragments; ++j ) {
      array[offset * i + j] = (i + 1) * offset + j;
    }
  }

  for( size_t i = 0; i < size / numberOfFragments; ++i ) {
    array[offset * (numberOfFragments - 1) + i] = i + 1;
  }
  array[(numberOfFragments - 1) * offset + (numberOfFragments - 1)] = 0;
}

void runThrough(int *array, size_t size, int numberOfPasses) {
  int i, k;
  for( i = 0, k = 0; i < size * numberOfPasses; ++i ) {
    k = array[k];
  }
  if( k == 253 ) {
    std::cout << "No optimizations";
  }
}

unsigned long long getTicks(int *array, size_t size, int numberOfPasses) {
  unsigned long long start, end;
  start = __rdtsc();
  runThrough(array, size, numberOfPasses);
  end = __rdtsc();
  return (end - start) / size / numberOfPasses;
}

void warmUpCache(size_t size, size_t numberOfFragments, size_t offset, int numberOfPasses) {
  int *array = new int[size];
  fillArray(array, size, numberOfFragments, offset);
  for( int i = 0; i < 256; ++i ) {
    runThrough(array, size, numberOfPasses);
  }
  delete[] array;
}

void warmUpCPU() {
  int *array = new int[16777216];
  for( int i = 0; i < 16777215; ++i ) {
    array[i] = i + 1;
  }
  array[16777215] = 0;

  for( int i = 0; i < 1024; ++i ) {
    runThrough(array, 16777216, 1);
  }

  delete[] array;
}

void printResults(int *array, size_t size, size_t numberOfFragments, size_t offset,
                  int numberOfPasses) {
  for( numberOfFragments = 1; numberOfFragments <= 32; ++numberOfFragments ) {
    int *array = new int[offset * numberOfFragments];
    fillArray(array, size, numberOfFragments, offset);
    unsigned long long result = getTicks(array, size, numberOfPasses);
    std::cout << numberOfFragments << " fragments: " << result << " ticks." << std::endl;
    delete[] array;
  }
}

int main(int argc, char **argv) {
  size_t numberOfFragments;
  size_t size = (L1_CACHE + L2_CACHE + L3_CACHE) / sizeof(size);
  size_t offset = OFFSET / sizeof(offset);

  int numberOfPasses = std::stoi(argv[1]);
  int *array;

  warmUpCPU();
  warmUpCache(size, numberOfFragments, offset, numberOfPasses);
  printResults(array, size, numberOfFragments, offset, numberOfPasses);

  return 0;
}
*/

#include <x86intrin.h>
#include <iostream>
#include <string>

#pragma intrinsic(__rdtsc)

#define L1_CACHE 32768  // 32 KB
#define L2_CACHE 524288 // 512 KB
#define L3_CACHE 4194304// 4 MB
#define OFFSET 16777216 // 16 MB

union ticks {
  unsigned long long t64;

  struct s32 {
    long th, tl;
  } t32;
};

void Clock(volatile ticks& time) {
  asm("rdtsc\n" : "=a"(time.t32.th), "=d"(time.t32.tl));
}

void fillArray(uint32_t *array, size_t size, size_t numberOfFragments, size_t offset) {
  for( size_t i = 0; i < numberOfFragments; ++i ) {
    for( size_t j = 0; j < size / numberOfFragments; ++j ) {
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

void runThrough(const uint32_t *array, size_t size, int numberOfPasses) {
  int i, k;
  for( i = 0, k = 0; i < size * numberOfPasses; ++i ) {
    k = array[k];
  }
  if( k == 253 ) {
    std::cout << "No optimizations! :)";
  }
}

unsigned long long getTicks(uint32_t *array, size_t size, int numberOfPasses) {

  ticks start{}, end{};
  Clock(start);
  runThrough(array, size, numberOfPasses);
  Clock(end);
  return (end - start) / size / numberOfPasses;
}

void warmUpCache(const uint32_t *array, size_t size, size_t numberOfFragments, size_t offset,
                 int numberOfPasses) {
  for( int i = 0; i < 10; ++i ) {
    runThrough(array, size, numberOfPasses);
  }
}

void warmUpCPU() {
  uint32_t *array = new int[16777216];
  for( int i = 0; i < 16777215; ++i ) {
    array[i] = i + 1;
  }
  array[16777215] = 0;

  for( int i = 0; i < 1024; ++i ) {
    runThrough(array, 16777216, 1);
  }
  delete[] array;
}

void printResults(size_t size, size_t numberOfFragments, size_t offset, int numberOfPasses) {
  for( numberOfFragments = 1; numberOfFragments <= 32; ++numberOfFragments ) {
    uint32_t *array = new uint32_t[offset * numberOfFragments];
    fillArray(array, size, numberOfFragments, offset);
    warmUpCache(size, numberOfFragments, offset, numberOfPasses);
    unsigned long long result = getTicks(array, size, numberOfPasses);
    std::cout << numberOfFragments << " fragments: " << result << " ticks." << std::endl;
    delete[] array;
  }
}

int main(int argc, char **argv) {
  size_t numberOfFragments;
  size_t size = (L1_CACHE + L2_CACHE + L3_CACHE) / sizeof(int);
  size_t offset = OFFSET / sizeof(int);

  int numberOfPasses = std::stoi(argv[1]);

  warmUpCPU();
  printResults(size, numberOfFragments, offset, numberOfPasses);

  return 0;
}
