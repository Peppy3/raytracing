#include <stdbool.h>

#include <string.h>
#include <math.h>

#include <def.h>
#include <interval.h>
#include <vec.h>
#include <ray.h>
#include <hittable.h>

#include <objects/triangle.h>


struct Triangle *Triangle_new(struct Triangle *tri, 
		const float_v3 verts[3], const float_v3 normal, struct Material *mat) {

	tri->hittable.hit = Triangle_hit;
	tri->mat = mat;

	tri->a = verts[0];
	tri->b = verts[1];
	tri->c = verts[2];

	tri->normal = normal;

	return tri;
}

bool Triangle_hit(const Hittable *obj, const Ray *r, 
		Interval ray_t, struct HitRecord *record) {
	struct Triangle *tri = (struct Triangle*)obj;

	float_v3 edge1 = float_v3_sub(tri->b, tri->a);
	float_v3 edge2 = float_v3_sub(tri->c, tri->a);
	float_v3 ray_cross_e2 = float_v3_cross(r->direction, edge2);
	float det = float_v3_dot(edge1, ray_cross_e2);

	if (det > -EPSILON && det < EPSILON) {
		return false; // this ray is parallel to the triangle
	}

	float inv_det = 1.0f / det;
	float_v3 s = float_v3_sub(r->origin, tri->a);
	float u = inv_det * float_v3_dot(s, ray_cross_e2);

	if (u < 0 || u > 1) {
		return false;
	}

	float_v3 s_cross_e1 = float_v3_cross(s, edge1);
	float v = inv_det * float_v3_dot(r->direction, s_cross_e1);

	if (v < 0 || u + v > 1) {
		return false;
	}
	
	// At this stage, we can calculate where the interseciton is on the line
	float t = inv_det * float_v3_dot(edge2, s_cross_e1);

	if (!Interval_contains(ray_t, t)) {
		return false;
	}
	
	record->t = t;
	record->position = float_v3_scale(float_v3_add(r->origin, r->direction), t);

	HitRecord_set_face_normal(record, r, tri->normal);
	record->mat = tri->mat;

	return true;
}
