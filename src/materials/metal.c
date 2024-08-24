#include <stdbool.h>

#include <vec.h>
#include <ray.h>
#include <hittable.h>

#include <materials/metal.h>

void Metal_new(struct Metal *metal, const float_v3 albedo, float fuzz) {
	metal->mat.scatter = Metal_scatter;
	metal->albedo = albedo;
	metal->fuzz = fuzz;
}

bool Metal_scatter(struct Material *mat, const Ray *r_in, 
		struct HitRecord *record, float_v3 *attenuation, Ray *scattered) {
	struct Metal *metal = (struct Metal*)mat;

	float_v3 reflected = float_v3_reflect(r_in->direction, record->normal);
	reflected = float_v3_add(
			float_v3_unit_vector(reflected),
			float_v3_scale(float_v3_rand_unit_vector(), metal->fuzz)
			);

	*scattered = (Ray){record->position, reflected};
	*attenuation = metal->albedo;
	return true;
}
