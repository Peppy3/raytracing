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
} Hittable;

// Sets the hit record of the normal vector.
// NOTE: the parameter 'outward_normal' is assumed to have unit length
void HitRecord_set_face_normal(struct HitRecord *record, 
		const Ray *r, float_v3 outward_normal);

#endif /* HITTABLE_H */

