#ifndef WORLD_H
#define WORLD_H

#include <camera.h>
#include <ray.h>
#include <hittable.h>

struct World {
	struct Camera **cameras;
	size_t camera_list_len;

	Hittable **obj_list;
	size_t obj_list_len;
};

bool World_hit(const struct World *world, const Ray *r, 
		Interval ray_t, struct HitRecord *record);


// The returned image should be freed with stdlib.h free when done with it
struct Image *World_render(struct World *world, size_t cam_idx);

#endif /* WORLD_H */

