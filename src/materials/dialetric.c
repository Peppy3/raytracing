#include <stdbool.h>

#include <math.h>

#include <def.h>
#include <vec.h>
#include <ray.h>
#include <hittable.h>

#include <materials/dialectric.h>


void Dialectric_new(struct Dialectric *d, float refraction_index) {
	d->mat.scatter = Dialectric_scatter;
	d->refraction_index = refraction_index;
}

static float Dialectric_reflectance(float cos, float refraction_index) {
	float r0 = (1.0f - refraction_index) / (1.0f + refraction_index);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * powf(1.0f - cos, 5.0f);
}

bool Dialectric_scatter(struct Material *mat, const Ray *r_in, 
		struct HitRecord *record, float_v3 *attenuation, Ray *scattered) {
	struct Dialectric *d = (struct Dialectric*)mat;
	
	*attenuation = (float_v3){{1.0f, 1.0f, 1.0f}};
	float ri = record->front_face ? (1.0f / d->refraction_index) : d->refraction_index;

	float_v3 unit_direction = float_v3_unit_vector(r_in->direction);
	float cos_theta = fminf(
			float_v3_dot(float_v3_neg(unit_direction), record->normal), 1.0f);
	float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);

	bool cannot_refract = (ri * sin_theta) > 1.0f;
	float_v3 direction;

	if (cannot_refract || Dialectric_reflectance(cos_theta, ri) > random_float()) {
		direction = float_v3_reflect(unit_direction, record->normal);
	}
	else {
		direction = float_v3_refract(unit_direction, record->normal, ri);
	}

	*scattered = (Ray){record->position, direction};
	return true;
}

