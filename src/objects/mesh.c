#include <stddef.h>
#include <stdbool.h>

#include <stdio.h>

#include <vec.h>

#include <hittable.h>
#include <objects/triangle.h>
#include <objects/mesh.h>

struct MeshTri {
	float_v3 normal;
	float_v3 verts[3];
};

struct Mesh {
	Hittable hittable;
	struct Material *mat;
	uint32_t num_tris;
	struct MeshTri tris[] ALIGNED;
};

struct PACKED STL_tri {
	float_v3 normal;
	float_v3 verts[3];
	uint16_t attr;
};

struct Mesh *Mesh_open_bin_stl(const char *filepath) {
	size_t bytes_read = 0;
	uint32_t num_tris = 0;
	FILE *fp = NULL;
	struct Mesh *mesh = NULL;

	fp = fopen(filepath, "rb");
	if (fp == NULL) {
		perror(filepath);
		return NULL;
	}

	fseek(fp, 80, SEEK_CUR);

	bytes_read = fread(&num_tris, 1, sizeof num_tris, fp);
	if (bytes_read < sizeof num_tris) {
		perror(filepath);
		fclose(fp);
		return NULL;
	}

	mesh = malloc(sizeof *mesh + num_tris * sizeof(struct MeshTri));
	if (mesh == NULL) {
		fclose(fp);
		return NULL;
	}

	mesh->num_tris = num_tris;

	for (uint32_t i = 0; i < num_tris; i++) {
		char tri[50];
		bytes_read = fread(&tri, 1, sizeof tri, fp);
		if (bytes_read < sizeof tri) {
			perror(filepath);
			free(mesh);
			fclose(fp);
			return NULL;
		}

		float_v3 *tmp = (float_v3*)tri;

		mesh->tris[i].normal   = tmp[0];
		mesh->tris[i].verts[0] = tmp[1];
		mesh->tris[i].verts[1] = tmp[2];
		mesh->tris[i].verts[2] = tmp[3];
	}

	fclose(fp);

	return mesh;
}

struct Mesh *Mesh_new(enum MeshFileType f_type, const char *filepath, 
		struct Material *mat) {
	struct Mesh *mesh = NULL;
	switch (f_type) {
		case MeshFileType_BinSTL: 
			{
				mesh = Mesh_open_bin_stl(filepath);
			} break;
		default:
			return NULL;
	}
	
	if (mesh == NULL) {
		return NULL;
	}
	
	mesh->hittable.hit = Mesh_hit;
	mesh->mat = mat;

	return mesh;
}

Hittable *Mesh_get_Hittable(struct Mesh *mesh) {
	return &mesh->hittable;
}

bool Mesh_hit(const Hittable *obj, const Ray *r, 
		Interval ray_t, struct HitRecord *record) {
	struct Mesh *mesh = (struct Mesh*)obj;

	struct Triangle tmp_tri = {
		.hittable.hit = Triangle_hit,
		.mat = mesh->mat,
	};
	struct HitRecord tmp_record = {0};
	bool hit_anyting = false;
	float closest_so_far = ray_t.max;

	for (size_t i = 0; i < mesh->num_tris; i++) {

		tmp_tri.normal = mesh->tris[i].normal;
		tmp_tri.a = mesh->tris[i].verts[0];
		tmp_tri.b = mesh->tris[i].verts[1];
		tmp_tri.c = mesh->tris[i].verts[2];

		if (Triangle_hit(&tmp_tri.hittable, r, (Interval){ray_t.min, closest_so_far}, &tmp_record)) {
			hit_anyting = true;
			closest_so_far = tmp_record.t;
			*record = tmp_record;
		}
	}

	return hit_anyting;
}

