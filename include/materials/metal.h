#ifndef METAL_H
#define METAL_H

#include <stdbool.h>

#include <vec.h>
#include <ray.h>
#include <hittable.h>
#include <material.h>

struct Metal {
	struct Material mat;
	float_v3 albedo;
	float fuzz;
};

void Metal_new(struct Metal *metal, const float_v3 albedo, float fuzz);

bool Metal_scatter(struct Material *mat, const Ray *r_in, 
		struct HitRecord *record, float_v3 *attenuation, Ray *scattered);

#endif /* METAL_H */

