#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdbool.h>

#include <vec.h>
#include <ray.h>
#include <object.h>
#include <hittable.h>

struct Material {
	bool (*scatter)(struct Material *mat, const Ray *r_in, struct HitRecord *record,
			float_v3 *attenuation, Ray *scattered);
};

struct Shader {
	bool (*diffuse)(struct Shader *shader, Ray *r, struct HitInfo *hit_info);
};

#endif /* MATERIAL_H */

