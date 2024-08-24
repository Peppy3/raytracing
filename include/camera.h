#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>

#include <vec.h>
#include <ray.h>

struct Camera {
	uint32_t image_width;
	uint32_t image_height;

	uint32_t samples_per_pixel;
	uint32_t max_depth;
	float pixels_samples_scale;

	float focal_length;
	float vertical_fov;
	float viewport_height;
	float viewport_width;

	point3 center;
	float_v3 u, v, w;

	float_v3 viewport_v;
	float_v3 viewport_u;

	float_v3 pixel_delta_v;
	float_v3 pixel_delta_u;

	float_v3 viewport_upper_left;
	float_v3 pixel00_location;
};

struct CameraOptions {
	uint32_t image_width;
	uint32_t image_height;

	float vertical_fov;

	uint32_t samples_per_pixel;
	uint32_t max_depth;

	point3 center;
	point3 lookat;
	float_v3 vup;
};

void Camera_new(struct Camera *cam, const struct CameraOptions *options);

// Get a ray form the camera to image position x, y
void Camera_get_ray(const struct Camera *cam, Ray *r, uint32_t x, uint32_t y);

#endif /* CAMERA_H */

