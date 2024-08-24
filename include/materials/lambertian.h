#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include <stdbool.h>

#include <vec.h>
#include <ray.h>
#include <hittable.h>
#include <material.h>

struct Lambertian {
	struct Material mat;
	float_v3 albedo;
};

void Lambertian_new(struct Lambertian *lamb, const float_v3 albedo);

bool Lambertian_scatter(struct Material *mat, const Ray *r_in, 
		struct HitRecord *record, float_v3 *attenuation, Ray *scattered);
#endif /* LAMBERTIAN_H */

