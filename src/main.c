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
#include <objects/mesh.h>

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
	float pitch = 64.5593f;
	float roll = 46.6919f;

	struct CameraOptions cam_opts1 = {
		.image_width = image_width,
		.image_height = image_height,

		.vertical_fov = 28.0f,
		.focus_dist = 0.5f,
		.defocus_angle = 0.6f,

		.max_depth = 30,
		.samples_per_pixel = 64,
		
		.center = (point3){7.35889f, -6.92579f, 4.95831f},
		.rotation_quart = euler_to_quaternion(pitch, 0, roll),
	};

	struct Camera cam1;
	Camera_new(&cam1, &cam_opts1);

	struct Camera *cam_list[] = {&cam1};
	
	// World

	struct Lambertian mat_ground, mat_mesh;

	Lambertian_new(&mat_ground, (float_v3){0.8f, 0.8f, 0.0f});
	Lambertian_new(&mat_mesh, (float_v3){0.6f, 0.4f, 0.8f});
	
	struct Mesh *cube = Mesh_new(MeshFileType_BinSTL, "cube.stl", &mat_mesh.mat);
	if (cube == NULL) {
		return 1;
	}
	
	/*
	Hittable tri_hit = {Triangle_hit};
	//const size_t cube_tris = 12;
	struct Triangle tris[12] = {
		{tri_hit, &mat_mesh.mat, { 1, -1, -1}, {-1, -1, -1}, {-1,  1, -1}, { 0,  0, -1}},
		{tri_hit, &mat_mesh.mat, { 1, -1, -1}, { 1,  1, -1}, {-1,  1, -1}, { 0,  0, -1}},
		
		{tri_hit, &mat_mesh.mat, {-1,  1,  1}, { 1,  1, -1}, { 1,  1,  1}, { 0,  1,  0}},
		{tri_hit, &mat_mesh.mat, {-1,  1,  1}, { 1,  1, -1}, {-1,  1, -1}, { 0,  1,  0}},

		{tri_hit, &mat_mesh.mat, {-1, -1,  1}, {-1,  1, -1}, {-1, -1, -1}, {-1,  0,  0}},
		{tri_hit, &mat_mesh.mat, {-1, -1,  1}, {-1,  1, -1}, {-1,  1,  1}, {-1,  0,  0}},

		{tri_hit, &mat_mesh.mat, { 1,  1, -1}, { 1, -1,  1}, { 1,  1,  1}, { 1,  0,  0}},
		{tri_hit, &mat_mesh.mat, { 1,  1, -1}, { 1, -1,  1}, { 1, -1, -1}, { 1,  0,  0}},
		
		{tri_hit, &mat_mesh.mat, { 1, -1, -1}, {-1, -1,  1}, {-1, -1, -1}, { 0, -1,  0}},
		{tri_hit, &mat_mesh.mat, { 1, -1, -1}, {-1, -1,  1}, { 1, -1,  1}, { 0, -1,  0}},

		{tri_hit, &mat_mesh.mat, {-1,  1,  1}, { 1, -1,  1}, { 1,  1,  1}, { 0,  0,  1}},
		{tri_hit, &mat_mesh.mat, {-1,  1,  1}, {-1, -1,  1}, { 1, -1,  1}, { 0,  0,  1}},
	};
	*/
	
	struct Sphere s_ground;
	Sphere_new(&s_ground, (point3){0, 0, -101.0f}, 100.0f, &mat_ground.mat);

	Hittable *hittable_list[] = {
		&s_ground.hittable, 
		Mesh_get_Hittable(cube),
		/*&tris[0].hittable,
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
		&tris[11].hittable*/
	};

	struct World world = {
		.cameras = cam_list,
		.camera_list_len = 1,
		.obj_list = hittable_list,
		.obj_list_len = 2
	};


	// Render
	
	struct Image *img = World_render(&world, 0);
	if (img == NULL) {
		fprintf(stderr, "Out of memory!\n");
		return 1;
	}
	
	puts("Saving image\x07");
	Image_save_as_ppm(img, "out.ppm", &cam_opts1);

	free(img);

	return 0;
}

