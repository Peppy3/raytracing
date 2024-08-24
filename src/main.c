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

#include <materials/lambertian.h>
#include <materials/metal.h>
#include <materials/dialectric.h>


int main(void) {
	
	const float image_aspect_ratio = 16.0/9.0;
	const uint32_t image_width = 400;
	
	// calculate the image height, make sure it's at least 1
	uint32_t image_height = (uint32_t)(image_width / image_aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// Camera

	struct CameraOptions cam_opts1 = {
		.image_width = image_width,
		.image_height = image_height,

		.focal_length = 1.0f,
		.max_depth = 100,
		.samples_per_pixel = 1024,
		.camera_center = (point3){0.0f, 0.0f, 0.0f}
	};

	struct Camera cam1;
	Camera_new(&cam1, &cam_opts1);

	struct Camera *cam_list[] = {&cam1};
	
	// World

	struct Lambertian mat_ground, mat_center;
	struct Metal mat_right;
	struct Dialectric mat_left, mat_bubble;

	Lambertian_new(&mat_ground, (float_v3){0.8f, 0.8f, 0.0f});
	Lambertian_new(&mat_center, (float_v3){0.1f, 0.2f, 0.5f});
	Dialectric_new(&mat_left, 1.50f);
	Dialectric_new(&mat_bubble, 1.0f / 1.33f);
	Metal_new(&mat_right, (float_v3){0.8f, 0.6f, 0.2f}, 1.0f);
	
	struct Sphere s_ground, s_center, s_left, s_bubble, s_right;
	Sphere_new(&s_ground, (point3){0.0f, -100.5f, -1.0f}, 100.0f, &mat_ground.mat);
	Sphere_new(&s_center, (point3){0.0f, 0.0f, -1.2f},    0.5f, &mat_center.mat);
	Sphere_new(&s_left,   (point3){1.0f, 0.0f, -1.0f},    0.5f, &mat_left.mat);
	Sphere_new(&s_bubble, (point3){1.0f, 0.0f, -1.0f},    0.4f, &mat_bubble.mat);
	Sphere_new(&s_right,  (point3){-1.0f, 0.0f, -1.0f},   0.5f, &mat_right.mat);

	Hittable *hittable_list[] = {
		&s_ground.hittable, 
		&s_center.hittable,
		&s_left.hittable,
		&s_bubble.hittable,
		&s_right.hittable
	};

	struct World world = {
		.cameras = cam_list,
		.camera_list_len = 1,
		.obj_list = hittable_list,
		.obj_list_len = 5
	};


	// Render
	
	struct Image *img = World_render(&world, 0);
	if (img == NULL) {
		fprintf(stderr, "Out of memory!");
		return 1;
	}

	Image_save_as_bmp(img, "out.bmp");

	free(img);

	return 0;
}

