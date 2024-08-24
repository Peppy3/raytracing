#ifndef DIALETRIC_H
#define DIALETRIC_H

#include <stdbool.h>

#include <vec.h>
#include <ray.h>
#include <hittable.h>
#include <material.h>

struct Dialectric {
	struct Material mat;
	float refraction_index;
};

void Dialectric_new(struct Dialectric *d, float refraction_index);

bool Dialectric_scatter(struct Material *mat, const Ray *r_in, 
		struct HitRecord *record, float_v3 *attenuation, Ray *scattered);

#endif /* DIALETRIC_H */

