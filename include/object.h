#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <stdbool.h>

#include <vec.h>
#include <ray.h>

struct HitInfo {
	float_v3 position, normal;
	float_v2 uv;
};

struct Object;

typedef bool (*intersection_func)(const struct Object *obj, const Ray *r,
	float *t, struct HitInfo *hit_info);

struct Object {
	intersection_func intersection;
	struct Shader *shader;
	struct {float_v3 corners[2];} aa_bounding_box; // axis aliged bounding box
};

// axis aliged bounding box
void Object_aa_bounding_box_intersection(const Ray *r, size_t num_objs, 
	struct Object **objs, float ts[]);

#endif /* OBJECT_H */
