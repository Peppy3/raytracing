#include <stdbool.h>

#include <vec.h>
#include <ray.h>
#include <hittable.h>

#include <materials/lambertian.h>


void Lambertian_new(struct Lambertian *lamb, const float_v3 albedo) {
	lamb->mat.scatter = Lambertian_scatter;
	lamb->albedo = albedo;
}

bool Lambertian_scatter(struct Material *mat, const Ray *r_in, 
		struct HitRecord *record, float_v3 *attenuation, Ray *scattered) {
	struct Lambertian *lamb = (struct Lambertian*)mat;

	float_v3 scatter_direction = float_v3_add(record->normal, float_v3_rand_unit_vector());
	if (float_v3_near_zero(scatter_direction)) {
		scatter_direction = record->normal;
	}
	*scattered = (Ray){record->position, scatter_direction};
	
	if (!record->front_face) {
		return false;
	}
	*attenuation = lamb->albedo;
	return true;
}
