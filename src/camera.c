#include <def.h>
#include <vec.h>
#include <ray.h>

#include "camera.h"
/*
void Camera_new(struct Camera *cam, 
		uint32_t image_width, uint32_t image_height, 
		float focal_length, uint32_t samples_per_pixel,
		point3 camera_center) {*/
void Camera_new(struct Camera *cam, const struct CameraOptions *options) {

	cam->image_width = options->image_width;
	cam->image_height = options->image_height;

	cam->samples_per_pixel = options->samples_per_pixel;
	cam->pixels_samples_scale = 1.0f / options->samples_per_pixel;
	cam->max_depth = options->max_depth;

	cam->focal_length = options->focal_length;

	// viewport heights less than one are ok since they are floats
	cam->viewport_height = 2.0f;
	cam->viewport_width = cam->viewport_height * ((float)options->image_width / options->image_height);

	cam->camera_center = options->camera_center;
	
	// calculate the vectors across the horizontal and vertical viewport edges
	cam->viewport_u = (float_v3) {cam->viewport_width, 0.0f, 0.0f};
	cam->viewport_v = (float_v3) {0.0f, -(cam->viewport_height), 0.0f};
	
	// calculate the delta vectors from pixel to pixel
	cam->pixel_delta_u = float_v3_div(cam->viewport_u, options->image_width);
	cam->pixel_delta_v = float_v3_div(cam->viewport_v, options->image_height); 
	
	// calulate the position of the upper left pixel
	cam->viewport_upper_left = float_v3_sub(
			float_v3_sub(
				float_v3_sub(
					options->camera_center,
					(float_v3){0.0f, 0.0f, options->focal_length}),
				float_v3_div(cam->viewport_u, 2.0f)),
			float_v3_div(cam->viewport_v, 2.0f)
			);
	cam->pixel00_location = float_v3_add(cam->viewport_upper_left,
			float_v3_scale(float_v3_add(cam->pixel_delta_u, cam->pixel_delta_v), 0.5f));
}

// TODO: sample disk instead
static float_v3 sample_square(const struct Camera *cam) {
	return float_v3_add(
			cam->camera_center,
			(float_v3){random_float() - 0.5f, random_float() - 0.5f, 0.0f}
			);
}

static float_v3 sample_disk(const struct Camera *cam) {
	return float_v3_add(
			cam->camera_center,
			float_v3_rand_in_unit_disk()
			);
}

void Camera_get_ray(const struct Camera *cam, Ray *r, uint32_t x, uint32_t y) {
	float_v3 offset = sample_disk(cam);

	float_v3 pixel_sample = float_v3_add(
			float_v3_add(
				float_v3_scale(cam->pixel_delta_u, (x + offset.x)),
				float_v3_scale(cam->pixel_delta_v, (y + offset.y))
				),
			cam->pixel00_location
			);

	float_v3 ray_origin = cam->camera_center;
	float_v3 ray_direction = float_v3_sub(pixel_sample, ray_origin);


	*r = (Ray){ray_origin, ray_direction};
}

