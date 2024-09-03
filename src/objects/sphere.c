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


struct Sphere2 *Sphere_new2(struct Sphere2 *s, point3 center, float radius, struct Shader *shader) {
	s->obj.aa_bounding_box.corners[0] = float_v3_sub(center, (float_v3){{radius, radius, radius}});
	s->obj.aa_bounding_box.corners[1] = float_v3_add(center, (float_v3){{radius, radius, radius}});

	s->obj.intersection = Sphere_hit2;
	s->obj.shader = shader;
	s->center = center;
	s->radius = fmaxf(0.0f, radius);
	return s;
}

bool Sphere_hit2(const struct Object *obj, const Ray *r,
	float *t, struct HitInfo *hit_info) {
	struct Sphere2 *s = (struct Sphere2 *)obj;
	
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
	Interval ray_t = {0.001f, INFINITY};
	float root = (h - sqrtd) / a;
	if (!Interval_surrounds(ray_t, root)) {
		root = (h + sqrtd) / a;
		if (!Interval_surrounds(ray_t, root)) {
			return false;
		}
	}

	float_v3 position = float_v3_add(r->origin, float_v3_scale(r->direction, root));
	float_v3 outward_normal = float_v3_div(float_v3_sub(position, s->center), s->radius);

	//bool back_face = float_v3_dot(r->direction, outward_normal) >= 0.0f;

	*t = root;
	hit_info->position = position;
	hit_info->normal = outward_normal;
	hit_info->uv = (float_v2){{0, 0}};

	return true;
}
/*
bool Sphere_hit3(const struct Object *obj, const Ray *r,
	float *t, struct HitInfo *hit_info) {
	
	struct Sphere2 *s = (struct Sphere2 *)obj;
	float_v3 oc = float_v3_sub(s->center, r->origin);

	float_v3 b = float_v3_dot(oc, r->direction);
	float_v3 qc = float_v3_sub(oc, float_v3_scale(b, r->direction));
	float h = (s->radius * s->radius) - float_v3_dot(qc, qc);

	if (h < 0.0f) {
		return false;
	}

	h = sqrtf(h);

	float root = -b-h;

	float_v3 position = float_v3_add(r->origin, float_v3_scale(r->direction, root));
	float_v3 outward_normal = float_v3_div(float_v3_sub(position, s->center), s->radius);

	return true;
}
*/
