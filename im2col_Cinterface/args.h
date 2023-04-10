#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#define CHECK(condition) _CHECK(condition, #condition)
#define _CHECK(con, msg)              \
  if (!(con)) {                       \
    printf("check %s failed\n", msg); \
    exit(1);                          \
  }

struct stride_args {
  int h;
  int w;
};

struct dilation_args {
  // temporary not think of this
  int h;
  int w;
};

struct padding_args {
  int up;
  int down;
  int left;
  int right;
};

struct image_args {
  int        mb;
  int        ic;
  int        h;
  int        w;
  inline int size() const { return mb * ic * h * w; }
  inline int size() { return mb * ic * h * w; }
};

struct kernel_args {
  int        oc;
  int        ic;
  int        h;
  int        w;
  inline int size() const { return oc * ic * h * w; }
  inline int size() { return oc * ic * h * w; }
};

#endif
