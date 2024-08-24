#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdbool.h>

#include <vec.h>
#include <ray.h>
#include <hittable.h>

struct Material {
	bool (*scatter)(struct Material *mat, const Ray *r_in, struct HitRecord *record,
			float_v3 *attenuation, Ray *scattered);
};

#endif /* MATERIAL_H */

