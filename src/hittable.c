#include <stdbool.h>

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
