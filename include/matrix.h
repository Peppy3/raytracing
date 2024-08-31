#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

#include <vec.h>

void Matrix_mul(size_t h, size_t w, float lhs[h][w], float rhs[h][w], float res[h][w]);

float_v3 Matrix_float_v3_mul(float lhs[3][3], float_v3 rhs);

#endif /* MATRIX_H */

