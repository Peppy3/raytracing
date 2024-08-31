#include <def.h>
#include <vec.h>
#include <ray.h>

#include "camera.h"

void Camera_new(struct Camera *cam, const struct CameraOptions *options) {
	cam->image_width = options->image_width;
	cam->image_height = options->image_height;

	cam->samples_per_pixel = options->samples_per_pixel;
	cam->pixels_samples_scale = 1.0f / options->samples_per_pixel;
	cam->max_depth = options->max_depth;
	
	cam->defocus_angle = options->defocus_angle;
	cam->center = options->center;

	// viewport heights less than one are ok since they are floats
	float theta = degrees_to_radians(options->vertical_fov);
	float h = tanf(theta / 2);
	cam->viewport_height = 2.0f * h * options->focus_dist;
	cam->viewport_width = cam->viewport_height * ((float)options->image_width / options->image_height);

	// Calculate the u (rigth), v (up), w (backwars) unit basis vectors for the camera coordinate frame.
	cam->w = float_v3_rotate_by_quaternion((float_v3){0, 0, 1}, options->rotation_quart);
	//cam->v = float_v3_rotate_by_quaternion((float_v3){0, 1, 0}, options->rotation_quart);
	cam->u = float_v3_rotate_by_quaternion((float_v3){1, 0, 0}, options->rotation_quart);
	cam->v = float_v3_cross(cam->w, cam->u);

	// calculate the vectors across the horizontal and vertical viewport edges
	cam->viewport_u = float_v3_scale(cam->u, cam->viewport_width);
	cam->viewport_v = float_v3_scale(float_v3_neg(cam->v), cam->viewport_height);

	// calculate the delta vectors from pixel to pixel
	cam->pixel_delta_u = float_v3_div(cam->viewport_u, options->image_width);
	cam->pixel_delta_v = float_v3_div(cam->viewport_v, options->image_height); 
	
	// calulate the position of the upper left pixel
	cam->viewport_upper_left = float_v3_sub(
			float_v3_sub(
				float_v3_sub(
					options->center,
					float_v3_scale(cam->w, options->focus_dist)),
				float_v3_div(cam->viewport_u, 2.0f)),
			float_v3_div(cam->viewport_v, 2.0f)
			);
	cam->pixel00_location = float_v3_add(cam->viewport_upper_left,
			float_v3_scale(float_v3_add(cam->pixel_delta_u, cam->pixel_delta_v), 0.5f));

	// Calculate the camera defocus disk basis vectors
	float defocus_radius = options->focus_dist * tanf(degrees_to_radians(options->defocus_angle / 2));
	cam->defocus_disk_u = float_v3_scale(cam->u, defocus_radius);
	cam->defocus_disk_v = float_v3_scale(cam->v, defocus_radius);
}
/*
void Camera_new_old(struct Camera *cam, const struct CameraOptions *options) {

	cam->image_width = options->image_width;
	cam->image_height = options->image_height;

	cam->samples_per_pixel = options->samples_per_pixel;
	cam->pixels_samples_scale = 1.0f / options->samples_per_pixel;
	cam->max_depth = options->max_depth;
	
	cam->defocus_angle = options->defocus_angle;
	cam->center = options->center;

	// viewport heights less than one are ok since they are floats
	float theta = degrees_to_radians(options->vertical_fov);
	float h = tanf(theta / 2);
	cam->viewport_height = 2.0f * h * options->focus_dist;
	cam->viewport_width = cam->viewport_height * ((float)options->image_width / options->image_height);

	// Calculate the u (rigth), v (up), w (backwards) unit basis vectors for the camera coordinate frame.
	cam->w = float_v3_unit_vector(float_v3_sub(options->center, options->lookat));
	cam->u = float_v3_unit_vector(float_v3_cross(options->vup, cam->w));
	cam->v = float_v3_cross(cam->w, cam->u);
	
	// calculate the vectors across the horizontal and vertical viewport edges
	cam->viewport_u = float_v3_scale(cam->u, cam->viewport_width);
	cam->viewport_v = float_v3_scale(cam->v, cam->viewport_height);

	// calculate the delta vectors from pixel to pixel
	cam->pixel_delta_u = float_v3_div(cam->viewport_u, options->image_width);
	cam->pixel_delta_v = float_v3_div(cam->viewport_v, options->image_height); 
	
	// calulate the position of the upper left pixel
	float_v3 viewport_upper_left = float_v3_sub(
			float_v3_sub(
				float_v3_sub(
					options->center,
					float_v3_scale(cam->w, options->focus_dist)),
				float_v3_div(cam->viewport_u, 2.0f)),
			float_v3_div(cam->viewport_v, 2.0f)
			);
	cam->pixel00_location = float_v3_add(viewport_upper_left,
			float_v3_scale(float_v3_add(cam->pixel_delta_u, cam->pixel_delta_v), 0.5f));

	// Calculate the camera defocus disk basis vectors
	float defocus_radius = options->focus_dist * tanf(degrees_to_radians(options->defocus_angle / 2));
	cam->defocus_disk_u = float_v3_scale(cam->u, defocus_radius);
	cam->defocus_disk_v = float_v3_scale(cam->v, defocus_radius);
}
*/
static float_v3 sample_square(const struct Camera *cam) {
	return float_v3_add(
			cam->center,
			(float_v3){random_float() - 0.5f, random_float() - 0.5f, 0.0f}
			);
}

static float_v3 sample_disk(const struct Camera *cam) {
	return float_v3_add(
			cam->center,
			float_v3_rand_in_unit_disk()
			);
}

// returns a random point in the camera defocus disk
static point3 defocus_disk_sample(const struct Camera *cam) {
	point3 p = float_v3_rand_in_unit_disk();
	return float_v3_add(cam->center,
			float_v3_add(
				float_v3_scale(cam->defocus_disk_u, p.x),
				float_v3_scale(cam->defocus_disk_v, p.y)
				)
			);
}

void Camera_get_ray(const struct Camera *cam, Ray *r, uint32_t x, uint32_t y) {
	// Construct a camera ray originating from the defocus disk and directed at a randomly
	// sampled point around the pixel location i, j.
	float_v3 offset = sample_disk(cam);

	float_v3 pixel_sample = float_v3_add(
			float_v3_add(
				float_v3_scale(cam->pixel_delta_u, (x + offset.x)),
				float_v3_scale(cam->pixel_delta_v, (y + offset.y))
				),
			cam->pixel00_location
			);

	float_v3 ray_origin = cam->center; //(cam->defocus_angle <= 0.0f) ? cam->center : defocus_disk_sample(cam);
	float_v3 ray_direction = float_v3_sub(pixel_sample, ray_origin);

	*r = (Ray){ray_origin, ray_direction};
}

