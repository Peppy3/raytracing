#ifndef OBJECT_SPHERE_H
#define OBJECT_SPHERE_H

#include <stdbool.h>

#include <hittable.h>
#include <material.h>
#include <vec.h>

struct Sphere {
	Hittable hittable;
	struct Material *mat;
	float_v3 center;
	float radius;
};

// returns the passed in sphere
struct Sphere *Sphere_new(struct Sphere *s, point3 center, float radius, struct Material *mat);

bool Sphere_hit(const Hittable *obj, const Ray *r, 
		Interval ray_t, struct HitRecord *record);

#endif /* OBJECT_SPHERE_H */

