#include <stdbool.h>

#include <math.h>

#include <interval.h>
#include <vec.h>
#include <ray.h>
#include <hittable.h>

#include <objects/sphere.h>

struct Sphere *Sphere_new(struct Sphere *s, point3 center, float radius, struct Material *mat) {
	s->hittable.hit = Sphere_hit;
	s->mat = mat;
	s->center = center;
	s->radius = fmaxf(0.0f, radius);
	return s;
}

bool Sphere_hit(const Hittable *obj, const Ray *r, 
		Interval ray_t, struct HitRecord *record) {
	struct Sphere *s = (struct Sphere *)obj;
	
	float_v3 oc = float_v3_sub(s->center, r->origin);
	float a = float_v3_length_squared(r->direction);
	float h = float_v3_dot(r->direction, oc);
	float c = float_v3_length_squared(oc) - (s->radius * s->radius);

	float discriminant = (h * h) - (a * c);
	if (discriminant < 0.0f) {
		return false;
	}

	float sqrtd = sqrtf(discriminant);

	// Find the nearest root that lies in the acceptable range
	float root = (h - sqrtd) / a;
	if (!Interval_surrounds(ray_t, root)) {
		root = (h + sqrtd) / a;
		if (!Interval_surrounds(ray_t, root)) {
			return false;
		}
	}

	record->t = root;
	record->position = Ray_at(r, record->t);
	float_v3 outward_normal = float_v3_div(float_v3_sub(record->position, s->center), s->radius);
	HitRecord_set_face_normal(record, r, outward_normal);
	record->mat = s->mat;
	
	return true;
}

