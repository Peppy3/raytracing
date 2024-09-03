#include <stddef.h>
#include <stdbool.h>

#include <math.h>

#include <vec.h>
#include <ray.h>

#include <hittable.h>

// Sets the hit record of the normal vector.
// NOTE: the parameter 'outward_normal' is assumed to have unit length
void HitRecord_set_face_normal(struct HitRecord *record, 
		const Ray *r, float_v3 outward_normal) {
	
	record->front_face = float_v3_dot(r->direction, outward_normal) < 0.0f;
	record->normal = (record->front_face) ? outward_normal : float_v3_scale(outward_normal, -1.0f);
}

void Hittable_aa_bounding_box_intersection(const Ray *r, size_t n_objs, 
	const Hittable *objs[n_objs], float ts[n_objs]) {
	
	bool signs[3] = {
		signbit(r->direction.x),
		signbit(r->direction.y),
		signbit(r->direction.z),
	};

	for (size_t i = 0; i < n_objs; i++) {
		const Hittable *obj = objs[i];
		float tmin = 0.0f, tmax = ts[i];

		for (size_t d = 0; d < 3; d++) {
			float bmin = obj->aa_bounding_box.corners[signs[d]].e[d];
			float bmax = obj->aa_bounding_box.corners[!signs[d]].e[d];

			float r_inv_dir = 1 / r->direction.e[d];
			float dmin = (bmin - r->origin.e[d]) * r_inv_dir;
			float dmax = (bmax - r->origin.e[d]) * r_inv_dir;

			tmin = fmaxf(bmin, dmin);
			tmax = fminf(bmax, dmax);
		}

		ts[i] = (tmin <= tmax) ? tmin : ts[i];
	}
		
}
