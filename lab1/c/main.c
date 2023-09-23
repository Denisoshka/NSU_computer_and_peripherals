#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>// for times
#include <unistd.h>   // for sysconf

long double sin_x(double x, long long int n) {
  long double start_sum = x;
  long double sum = 0.0;

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
  double rez = sin_x(x, n);
  times(&end);
  clocks = end.tms_utime - start.tms_utime;
  printf("Time taken: %lf sec.\n", (double) clocks / clocks_per_sec);
  fprintf(stdout, "rez %lf\n", rez);
  return 0;
}
