#include <stdio.h>
#include <math.h>
#include <sys/times.h> // for times
#include <unistd.h> // for sysconf
#include <stdlib.h>

long double fact(size_t n) {
  long double rez = 1;
  for (size_t i = 1; i <= n; ++i) {
    rez *= i;
  }
  return rez;
}

double sin_x(const double x,const size_t n) {
  double rez = 0;
  for (int i = 1, sign = 1; i < n; i += 2, sign *= -1) {
    rez += sign * (pow(x, i) / fact(i));
  }
  return rez;
}


int main(int argc, char** argv) {
  long long int n = atoll(argv[1]);
  double x = atof(argv[2]);

  struct tms start, end;
  long clocks_per_sec = sysconf(_SC_CLK_TCK);
  long clocks;
  times(&start);
  double rez = sin_x(x, n);
  times(&end);
  clocks = end.tms_utime - start.tms_utime;
  printf("Time taken: %lf sec.\n", (double)clocks / clocks_per_sec);
  fprintf(stdout, "rez %lf\n", rez);
  return 0;
}
