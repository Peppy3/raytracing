#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>

#include <def.h>
#include <image.h>
#include <camera.h>
#include <world.h>

#include <objects/sphere.h>
#include <objects/triangle.h>

#include <materials/lambertian.h>
#include <materials/metal.h>
#include <materials/dialectric.h>


int main(void) {
	
	const float image_aspect_ratio = 16.0/9.0;
	const uint32_t image_width = 640;
	
	// calculate the image height, make sure it's at least 1
	uint32_t image_height = (uint32_t)(image_width / image_aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// Camera

	// TODO: full rotation in degrees and pass that to the camera
	float yaw = 64.5593f;
	float pitch = 46.6919f;

	struct CameraOptions cam_opts1 = {
		.image_width = image_width,
		.image_height = image_height,

		.vertical_fov = 20.0f,
		.focus_dist = 1.2f,
		.defocus_angle = 0.6f,

		.max_depth = 30,
		.samples_per_pixel = 64,

		.center = (point3){7.35889f, -6.92579f, 4.95831f},
		.lookat = (point3){
			.x = cosf(yaw) * cosf(pitch), 
			.y = sinf(yaw) * cosf(pitch), 
			.z = sin(pitch)
		},
		.vup = (float_v3){0.0f, 1.0f, 0.0f}
	};

	struct Camera cam1;
	Camera_new(&cam1, &cam_opts1);

	struct Camera *cam_list[] = {&cam1};
	
	// World

	struct Lambertian mat_ground, mat_tri;

	Lambertian_new(&mat_ground, (float_v3){0.8f, 0.8f, 0.0f});
	Lambertian_new(&mat_tri, (float_v3){0.6f, 0.4f, 0.8f});

	Hittable tri_hit = {Triangle_hit};
	//const size_t cube_tris = 12;
	struct Triangle tris[12] = {
		{tri_hit, &mat_tri.mat, { 1, -1, -1}, {-1, -1, -1}, {-1,  1, -1}, { 0,  0, -1}},
		{tri_hit, &mat_tri.mat, { 1, -1, -1}, { 1,  1, -1}, {-1,  1, -1}, { 0,  0, -1}},
		
		{tri_hit, &mat_tri.mat, {-1,  1,  1}, { 1,  1, -1}, { 1,  1,  1}, { 0,  1,  0}},
		{tri_hit, &mat_tri.mat, {-1,  1,  1}, { 1,  1, -1}, {-1,  1, -1}, { 0,  1,  0}},

		{tri_hit, &mat_tri.mat, {-1, -1,  1}, {-1,  1, -1}, {-1, -1, -1}, {-1,  0,  0}},
		{tri_hit, &mat_tri.mat, {-1, -1,  1}, {-1,  1, -1}, {-1,  1,  1}, {-1,  0,  0}},

		{tri_hit, &mat_tri.mat, { 1,  1, -1}, { 1, -1,  1}, { 1,  1,  1}, { 1,  0,  0}},
		{tri_hit, &mat_tri.mat, { 1,  1, -1}, { 1, -1,  1}, { 1, -1, -1}, { 1,  0,  0}},
		
		{tri_hit, &mat_tri.mat, { 1, -1, -1}, {-1, -1,  1}, {-1, -1, -1}, { 0, -1,  0}},
		{tri_hit, &mat_tri.mat, { 1, -1, -1}, {-1, -1,  1}, { 1, -1,  1}, { 0, -1,  0}},

		{tri_hit, &mat_tri.mat, {-1,  1,  1}, { 1, -1,  1}, { 1,  1,  1}, { 0,  0,  1}},
		{tri_hit, &mat_tri.mat, {-1,  1,  1}, {-1, -1,  1}, { 1, -1,  1}, { 0,  0,  1}},
	};
	
	struct Sphere s_ground;
	Sphere_new(&s_ground, (point3){0.0f, -100.5f, -1.0f}, 100.0f, &mat_ground.mat);

	Hittable *hittable_list[] = {
		//&s_ground.hittable, 
		&tris[0].hittable,
		&tris[1].hittable,
		&tris[2].hittable,
		&tris[3].hittable,
		&tris[4].hittable,
		&tris[5].hittable,
		&tris[6].hittable,
		&tris[7].hittable,
		&tris[8].hittable,
		&tris[9].hittable,
		&tris[10].hittable,
		&tris[11].hittable
	};

	struct World world = {
		.cameras = cam_list,
		.camera_list_len = 1,
		.obj_list = hittable_list,
		.obj_list_len = 12
	};


	// Render
	
	struct Image *img = World_render(&world, 0);
	if (img == NULL) {
		fprintf(stderr, "Out of memory!\n");
		return 1;
	}
	
	puts("Saving image\x07");
	Image_save_as_bmp(img, "out.bmp");

	free(img);

	return 0;
}

