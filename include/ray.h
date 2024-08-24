#ifndef RAY_H
#define RAY_H

#include "vec.h"

typedef struct {
	point3 origin;
	float_v3 direction;
} Ray;

point3 Ray_at(const Ray *r, float t);

#endif /* RAY_H */

