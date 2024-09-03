#ifndef HITTABLE_H
#define HITTABLE_H

#include <stdint.h>
#include <stdbool.h>

#include <interval.h>
#include <vec.h>
#include <ray.h>

struct Material;

struct HitRecord {
	point3 position;
	float_v3 normal;
	struct Material *mat;
	float t;
	bool front_face;
};

typedef struct Hittable_s {
	bool (*hit)(const struct Hittable_s *obj, const Ray *r, 
			Interval ray_t, struct HitRecord *record);
	struct {float_v3 corners[2];} aa_bounding_box; // axis aliged bounding box
} Hittable;

// Sets the hit record of the normal vector.
// NOTE: the parameter 'outward_normal' is assumed to have unit length
void HitRecord_set_face_normal(struct HitRecord *record, 
		const Ray *r, float_v3 outward_normal);

void Hittable_aa_bounding_box_intersection(const Ray *r, size_t n_objs, 
	const Hittable *objs[n_objs], float ts[n_objs]);

#endif /* HITTABLE_H */

