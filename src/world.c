#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include <def.h>
#include <interval.h>
#include <vec.h>
#include <ray.h>
#include <hittable.h>
#include <material.h>
#include <camera.h>
#include <image.h>

#include <world.h>

bool World_hit(const struct World *world, const Ray *r, 
		Interval ray_t, struct HitRecord *record) {

	struct HitRecord temp_record = {0};
	bool hit_anyting = false;
	float closest_so_far = ray_t.max;

	for (size_t i = 0; i < world->obj_list_len; i++) {
		Hittable *obj = world->obj_list[i];
		if (obj->hit(obj, r, (Interval){ray_t.min, closest_so_far}, &temp_record)) {
			hit_anyting = true;
			closest_so_far = temp_record.t;
			*record = temp_record;
		}
	}

	return hit_anyting;
}

static float_v3 ray_color(const struct World *world, uint32_t depth, const Ray *r) {
	if (depth <= 0) {
		return (float_v3){0.0f, 0.0f, 0.0f};
	}
	struct HitRecord hit_rec = {0};

	if (World_hit(world, r, (Interval){0.001f, INFINITY}, &hit_rec)) {
		Ray scattered = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
		float_v3 color_attentuation = {0.0f, 0.0f, 0.0f};

		if (hit_rec.mat->scatter(hit_rec.mat, r, &hit_rec, &color_attentuation, &scattered)) {
			return float_v3_vec_scale(color_attentuation, ray_color(world, depth - 1, &scattered));
		}
		return (float_v3){0.0f, 0.0f, 0.0f};
	}

	float_v3 unit_direction = float_v3_unit_vector(r->direction);
	float a = 0.5f * (unit_direction.y + 1.0f);
	return float_v3_add(
			float_v3_scale((float_v3){1.0f, 1.0f, 1.0f}, 1.0f - a),
			float_v3_scale((float_v3){0.5f, 0.7f, 1.0f}, a)
			);
}

// The returned image should be freed with stdlib.h free when done with it
struct Image *World_render(struct World *world, size_t cam_idx) {
	assert(cam_idx < world->camera_list_len);
	struct Camera *cam = world->cameras[cam_idx];
	struct Image *img = Image_new(cam->image_width, cam->image_height);
	if (img == NULL) {
		return NULL;
	}

	uint32_t image_width = Image_get_width(img);
	uint32_t image_height = Image_get_height(img);

	uint8_v4 *img_data = Image_get_data(img);

	for (uint32_t j = 0; j < image_height; j++) {
		printf("\x1b[1K\rScanlines remaining: %u", image_height - j);
		fflush(stdout);

		for (uint32_t i = 0; i < image_width; i++) {
			float_v4 pixel_color = {0.0f, 0.0f, 0.0f, 0.0f};

			for (uint32_t sample = 0; sample < cam->samples_per_pixel; sample++) {
				Ray r = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
				Camera_get_ray(cam, &r, i, j);
				
				float_v3 rc = ray_color(world, cam->max_depth, &r);

				pixel_color = float_v4_add(pixel_color, (float_v4){rc.x, rc.y, rc.z, 0.0f});
			}

			pixel_color = float_v4_scale(pixel_color, cam->pixels_samples_scale);

			Interval intensity = {0.0f, 0.999f};

			pixel_color.x = 256.0f * Interval_clamp(intensity, pixel_color.x);
			pixel_color.y = 256.0f * Interval_clamp(intensity, pixel_color.y);
			pixel_color.z = 256.0f * Interval_clamp(intensity, pixel_color.z);
			pixel_color.w = 256.0f; // no transparency

			img_data[j * image_width + i] = float_v4_to_uint8_v4(pixel_color);
		}
	}
	puts("\x1b[1K\rDone!");

	return img;
}

