#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>// for times
#include <unistd.h>   // for sysconf

long double sin_x(const double x, const size_t n) {
  long double rez = x;
  long double sign;
  long double prev_rez = x;

  for (int i = 3, sign = -1; i < n; i += 2, sign *= -1) {
    prev_rez = (sign * rez * prev_rez * prev_rez) / ((i - 1) * i);
    rez += prev_rez;
  }
  return rez;
}

long double sin_e2(double x, long long int n) {
  long double start_sum = x;
  long double sum = 0.0;
//  int i = 1;

  for (long long int i = 1; i < n; i++) {
    sum += start_sum;
    start_sum *= -1.0 * x * x / ((2 * i) * (2 * i + 1));
  }

  return sum;
}

int main(int argc, char** argv) {
  long long int n = atoll(argv[1]);
  double x = atof(argv[2]);

  struct tms start, end;
  long clocks_per_sec = sysconf(_SC_CLK_TCK);
  long clocks;
  times(&start);
  double rez = sin_e2(x, n);
  times(&end);
  clocks = end.tms_utime - start.tms_utime;
  printf("Time taken: %lf sec.\n", (double) clocks / clocks_per_sec);
  fprintf(stdout, "rez %lf\n", rez);
  return 0;
}
