#ifndef MESH_H
#define MESH_H

#include <stdbool.h>

#include <hittable.h>
#include <material.h>

struct Mesh;

enum MeshFileType {
	MeshFileType_BinSTL = 1,
};

struct Mesh *Mesh_new(enum MeshFileType f_type, const char *filepath, 
		struct Material *mat);

Hittable *Mesh_get_Hittable(struct Mesh *mesh);

bool Mesh_hit(const Hittable *obj, const Ray *r, 
		Interval ray_t, struct HitRecord *record);

#endif /* MESH_H */

