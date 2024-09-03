#ifndef WORLD_H
#define WORLD_H

#include <stddef.h>
#include <stdint.h>

#include <image.h>
#include <camera.h>
#include <ray.h>
#include <hittable.h>

struct World {
	float_v4 background_color;

	struct Camera **cameras;
	size_t camera_list_len;

	Hittable **obj_list;
	size_t obj_list_len;
};

struct World2 {
	float_v4 background_color;

	struct Camera **cameras;
	size_t camera_list_len;

	struct Object **obj_list;
	size_t obj_list_len;
};

//bool World_hit(const struct World *world2, const Ray *r, 
//		Interval ray_t, struct HitRecord *record);


// The returned image should be freed with stdlib.h free when done with it
struct Image *World_render(struct World2 *world, size_t cam_idx);

#endif /* WORLD_H */

