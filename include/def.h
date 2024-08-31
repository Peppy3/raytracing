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

static int pgc_rand_v = 13;

// Linear congruential generator
static inline void pgc_srand(int seed) {
	pgc_rand_v = seed * 1664525u + 1013904223u;
}

static inline int pgc_rand(void) {
	int state = pgc_rand_v * 747796405u + 2891336453u;
	pgc_rand_v = state;
	int word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
	return (word >> 22u) ^ word;
}

static inline float random_float(void) {
	return pgc_rand() / ((float)INT32_MAX + 1.0f);
}

static inline float random_float_min_max(float min, float max) {
	return min + (max - min) * random_float();
}

#endif /* DEF_H */

