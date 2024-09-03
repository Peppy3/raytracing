#ifndef OBJECT_SPHERE_H
#define OBJECT_SPHERE_H

#include <stdbool.h>

#include <object.h>
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


struct Sphere2 {
	struct Object obj;
	float_v3 center;
	float radius;
};

struct Sphere2 *Sphere_new2(struct Sphere2 *s, point3 center, float radius, struct Shader *shader);

bool Sphere_hit2(const struct Object *obj, const Ray *r,
	float *t, struct HitInfo *hit_info);

bool Sphere_hit3(const struct Object *obj, const Ray *r,
	float *t, struct HitInfo *hit_info);

#endif /* OBJECT_SPHERE_H */
