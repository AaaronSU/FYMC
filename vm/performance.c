//
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//
#if defined(__INTEL_COMPILER)
#include <mkl.h>
#else
#include <cblas.h>
#endif

//
#include "cpu.h"
#include "opcode.h"
#include <string.h>

//
#include "tools.h"

//
#define ALIGN64 64

//
#define MAX_SAMPLES 33

//
void run_benchmark(char *title,
                   void (*kernel)(CPU *cpu, Instruction inst),
                   CPU *cpu,
                   Instruction inst,
                   u64 n);

//
int main(int argc, char **argv)
{
  CPU cpu;
  memset(&cpu, 0, sizeof(CPU)); // Initialize cpu to zero
  Instruction inst;
  memset(&inst, 0, sizeof(inst));
  //
  srand(getpid());

  //
  if (argc < 2)
    return printf("usage: %s [n]\n", argv[0]), 1;

  // Number of kernel repetitions
  u64 n = atoll(argv[1]);

  // Print header
  printf("%10s;  %10s; %15s; %15s; %15s; %14s; %16s\n",
         "title", "min(ns)", "max(ns)", "n", "mean(ns)", "stddev(%)", "operation/s");

  run_benchmark("MOV", mov, &cpu, inst, n);
  run_benchmark("ADDU", addu, &cpu, inst, n);
  run_benchmark("ADDF", addf, &cpu, inst, n);
  run_benchmark("ADDS", adds, &cpu, inst, n);
  run_benchmark("SUBU", subu, &cpu, inst, n);
  run_benchmark("MULU", mulu, &cpu, inst, n);

  run_benchmark("STOREU", storeu, &cpu, inst, n);

  return 0;
}

//
void run_benchmark(char *title,
                   void (*kernel)(CPU *cpu, Instruction inst),
                   CPU *cpu,
                   Instruction inst,
                   u64 n)
{
  //
  double elapsed = 0.0;
  struct timespec t1, t2;
  double samples[MAX_SAMPLES];

  //
  for (u64 i = 0; i < MAX_SAMPLES; i++)
  {
    do
    {
      clock_gettime(CLOCK_MONOTONIC_RAW, &t1);

      for (u64 j = 0; j < n; j++)
        kernel(cpu, inst);

      clock_gettime(CLOCK_MONOTONIC_RAW, &t2);

      elapsed = (double)(t2.tv_nsec - t1.tv_nsec) / (double)n;
    } while (elapsed <= 0.0);

    samples[i] = elapsed;
  }

  //
  sort_f64(samples, MAX_SAMPLES);

  //
  double min = samples[0];
  double max = samples[MAX_SAMPLES - 1];
  double mean = mean_f64(samples, MAX_SAMPLES);
  double dev = stddev_f64(samples, MAX_SAMPLES);
  double oppersec = 1e9 / mean;

  //
  printf("%10s; %11.3lf; %15.3lf; %15llu; %15.3lf; %12.3lf%%; %17.2lf\n", title, min, max, (unsigned long long)n, mean, dev, oppersec);
}
