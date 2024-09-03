#ifndef DEF_H
#define DEF_H

#include <stdint.h>

#include <stdlib.h>
#include <math.h>

// random defines

#define PACKED __attribute__((__packed__))
#define ALIGNED __attribute__((aligned))

#define EPSILON 1e-8
#define PI 3.1415926535897932385

#define degrees_to_radians(d) ((d) * PI / 180.0)

// Linear congruential generator
void pgc_srand(int seed);

int pgc_rand(void);

float random_float(void);

float random_float_min_max(float min, float max);

#endif /* DEF_H */

