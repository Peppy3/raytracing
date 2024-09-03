#include <stdint.h>

#include <stdlib.h>
#include <math.h>

static unsigned long pgc_rand_v = 13;

void pgc_srand(int seed) {
	pgc_rand_v = seed * 1664525u + 1013904223u;
}

int pgc_rand(void) {
	int state = pgc_rand_v * 747796405u + 2891336453u;
	pgc_rand_v = state;
	int word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
	return (word >> 22u) ^ word;
}

float random_float(void) {
	return pgc_rand() / ((float)INT32_MAX + 1.0f);
}

float random_float_min_max(float min, float max) {
	return min + (max - min) * random_float();
}