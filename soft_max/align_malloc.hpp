#pragma once
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

float* malloc_aligned(int m, int n, int size) {
  float* ptr;
  int    err;

  err = posix_memalign((void**)&ptr, (size_t)64, size * m * n);

  if (err) {
    printf("bl_malloc_aligned(): posix_memalign() failures");
    exit(1);
  }

  return ptr;
}
