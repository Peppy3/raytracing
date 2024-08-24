#include <vec.h>

#include <ray.h>

point3 Ray_at(const Ray *r, float t) {
	return float_v3_add(r->origin, float_v3_scale(r->direction, t));
}

