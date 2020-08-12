#include "opencl-shim.h"
__kernel void A(__global float* a, __global float* b, __global float* c, const int d) {
  int e = get_global_id(0);

  if (e < c) {
    b[e] += 1;
    c[e] += c[e];
    c[e] += 1;
    a[e] = 2 * c[e] * c[e];
  }
}