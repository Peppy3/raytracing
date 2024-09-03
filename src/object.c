#include <stdint.h>
#include <stdbool.h>

#include <vec.h>
#include <ray.h>

#include <object.h>

// axis aliged bounding box
void Object_aa_bounding_box_intersection(const Ray *r, size_t n_objs, 
	struct Object **objs, float ts[]) {
	
	bool signs[3] = {
		signbit(r->direction.x),
		signbit(r->direction.y),
		signbit(r->direction.z),
	};

	for (size_t i = 0; i < n_objs; i++) {
		const struct Object *obj = objs[i];
		float tmin = 0.0f, tmax = ts[i];

		for (size_t d = 0; d < 3; d++) {
			float bmin = obj->aa_bounding_box.corners[signs[d]].e[d];
			float bmax = obj->aa_bounding_box.corners[!signs[d]].e[d];

			float r_inv_dir = 1 / r->direction.e[d];
			float dmin = (bmin - r->origin.e[d]) * r_inv_dir;
			float dmax = (bmax - r->origin.e[d]) * r_inv_dir;

			tmin = fmaxf(dmin, tmin);
			tmax = fminf(dmax, tmax);
		}

		ts[i] = (tmin <= tmax) ? tmin : ts[i];
	}
		
}