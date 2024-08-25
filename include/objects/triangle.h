#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdbool.h>

#include <hittable.h>
#include <material.h>
#include <vec.h>

// single triangle
struct Triangle {
	Hittable hittable;
	struct Material *mat;
	float_v3 a, b, c;
	float_v3 normal;
};

struct Triangle *Triangle_new(struct Triangle *tri, 
		const float_v3 verts[3], const float_v3 normal, struct Material *mat);

bool Triangle_hit(const Hittable *obj, const Ray *r, 
		Interval ray_t, struct HitRecord *record);

#endif /* TRIANGLE_H */

