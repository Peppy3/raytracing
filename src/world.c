#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include <stdlib.h>
#include <math.h>

#include <def.h>
#include <interval.h>
#include <vec.h>
#include <ray.h>
#include <hittable.h>
#include <object.h>
#include <material.h>
#include <camera.h>
#include <image.h>

#include <world.h>

static inline float linear_to_gamma(float linear_component) {
	if (linear_component > 0.0f) {
		return sqrtf(linear_component);
	}
	return 0.0f;
}
/*
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
		return (float_v3){{0.0f, 0.0f, 0.0f}};
	}
	struct HitRecord hit_rec = {0};

	if (World_hit(world, r, (Interval){0.001f, INFINITY}, &hit_rec)) {
		Ray scattered = {0};
		float_v3 color_attentuation = {{0.0f, 0.0f, 0.0f}};

		if (hit_rec.mat->scatter(hit_rec.mat, r, &hit_rec, &color_attentuation, &scattered)) {
			return float_v3_vec_scale(color_attentuation, ray_color(world, depth - 1, &scattered));
		}
		return (float_v3){{0.0f, 0.0f, 0.0f}};
	}

	float_v3 unit_direction = float_v3_unit_vector(r->direction);
	float a = 0.5f * (unit_direction.y + 1.0f);
	return float_v3_add(
			float_v3_scale((float_v3){{1.0f, 1.0f, 1.0f}}, 1.0f - a),
			float_v3_scale((float_v3){{0.5f, 0.7f, 1.0f}}, a)
			);
}
*/

struct Object *World_hit(const Ray *r, size_t num_objs, struct Object **objs, 
	float ts[], struct HitInfo *hit_info, float tmax) {

	Object_aa_bounding_box_intersection(r, num_objs, objs, ts);

	struct Object *was_hit = NULL;
	float t = tmax;
	for (size_t i = 0; i < num_objs; i++) {
		if (ts[i] < t) {
			if (objs[i]->intersection(objs[i], r, &t, hit_info)) {
				was_hit = objs[i];
			}
		}
	}

	return was_hit;
}

struct color_info {
	float_v4 color;
	float_v4 normal;
	float_v4 albedo;
};

static float *float_list_fill(size_t len, float list[static 1], float f) {
	for (size_t i = 0; i < len; i++) {
		list[i] = f;
	}
	return list;
}

int ray_color(const struct World2 *world, uint32_t depth, const Ray *r, struct color_info *colors) {
	if (depth <= 0) {
		colors->color = world->background_color;
		colors->normal = (float_v4){{0.0f, 0.0f, 0.0f, 0.0f}};
		colors->albedo = world->background_color;
		return 0;
	}

	float ts[world->obj_list_len];

	//float_v4 color = {0};
	struct HitInfo hit_info = {0};

	//for (size_t d = 0; d < depth; d++) {
		
		float_list_fill(world->obj_list_len, ts, INFINITY);

		struct Object *hit_object = World_hit(r, world->obj_list_len, world->obj_list, ts, &hit_info, INFINITY);
		if (hit_object == NULL) {
			colors->color = world->background_color;
			colors->normal = (float_v4){{0.0f, 0.0f, 0.0f, 0.0f}};
			colors->albedo = world->background_color;
			return 0;
		}

		//float_v3 attentuation = {{0.0f, 0.0f, 0.0f}};

		/*if (hit_object->shader->scatter()) {
			color = float_v3_vec_scale(color, attentuation);
		}*/
	//}

	colors->color = (float_v4){{hit_info.normal.x, hit_info.normal.y, hit_info.normal.z, 1.0f}};
	colors->normal = (float_v4){{hit_info.normal.x, hit_info.normal.y, hit_info.normal.z, 1.0f}};
	colors->albedo = (float_v4){{1.0f, 1.0f, 1.0f, 1.0f}};
	return 0;
}

// returns true if successful
bool sample_view(struct World2 *world, struct Camera *cam, 
	uint32_t image_width, uint32_t image_height, float_v4 color_data[]) {
	
	float *ts = malloc(image_width * image_height * sizeof(float));
	if (ts == NULL) {
		return false;
	}

	for (uint32_t j = 0; j < image_height; j++) {
		for (uint32_t i = 0; i < image_width; i++) {
			//float_v4 pixel_color = {{0.0f, 0.0f, 0.0f, 0.0f}};

			Ray r = {0};
			Camera_get_ray(cam, &r, i, j);
			
			struct color_info colors = {0};
			ray_color(world, cam->max_depth, &r, &colors);

			color_data[j * image_width + i] = colors.color;
		}
	}

	return true;
}


// The returned image should be freed with stdlib.h free when done with it
struct Image *World_render(struct World2 *world, size_t cam_idx) {
	assert(cam_idx < world->camera_list_len);
	struct Camera *cam = world->cameras[cam_idx];
	struct Image *img = Image_new(cam->image_width, cam->image_height);
	if (img == NULL) {
		return NULL;
	}
	/*
	uint32_t image_width = Image_get_width(img);
	uint32_t image_height = Image_get_height(img);

	uint8_v4 *img_data = Image_get_data(img);

	for (uint32_t j = 0; j < image_height; j++) {
		printf("\x1b[1K\rScanlines remaining: %u", image_height - j);
		fflush(stdout);

		for (uint32_t i = 0; i < image_width; i++) {
			float_v4 pixel_color = {{0.0f, 0.0f, 0.0f, 0.0f}};

			for (uint32_t sample = 0; sample < cam->samples_per_pixel; sample++) {
				Ray r = {0};
				Camera_get_ray(cam, &r, i, j);
				
				struct color_info colors = {0}; 
				ray_color(world, cam->max_depth, &r, &colors);

				pixel_color = float_v4_add(pixel_color, colors.color);
			}

			pixel_color = float_v4_scale(pixel_color, cam->pixels_samples_scale);

			pixel_color.x = linear_to_gamma(pixel_color.x);
			pixel_color.y = linear_to_gamma(pixel_color.y);
			pixel_color.z = linear_to_gamma(pixel_color.z);

			Interval intensity = {0.0f, 0.999f};
			pixel_color.x = 256.0f * Interval_clamp(intensity, pixel_color.x);
			pixel_color.y = 256.0f * Interval_clamp(intensity, pixel_color.y);
			pixel_color.z = 256.0f * Interval_clamp(intensity, pixel_color.z);
			pixel_color.w = 256.0f; // no transparency

			img_data[j * image_width + i] = float_v4_to_uint8_v4(pixel_color);
		}
	}
	puts("\x1b[1K\rDone!");
	
	*/

	uint32_t image_width = Image_get_width(img);
	uint32_t image_height = Image_get_height(img);

	float_v4 *color_data = malloc(image_width * image_height * sizeof(float_v4) * 2);
	if (color_data == NULL) {
		free(img);
		return NULL;
	}
	float_v4 *color_sample_data = &color_data[image_width * image_height];

	uint32_t samples_per_pixel = cam->samples_per_pixel;
	for (uint32_t sample_count = 0; sample_count < samples_per_pixel; sample_count++) {
		printf("\x1b[1K\rSamples remaining: %u", samples_per_pixel - sample_count);
		fflush(stdout);

		bool sample_success = sample_view(world, cam, image_width, image_height, color_sample_data);
		if (!sample_success) {
			free(img);
			free(color_data);
			return NULL;
		}

		for (size_t i = 0; i < image_width * image_height; i++) {
			color_data[i] = float_v4_add(color_data[i], color_sample_data[i]);
		}

	}

	// correct and convert data to final image
	uint8_v4 *img_data = Image_get_data(img);
	float pixels_samples_scale = 1.0f / (float)samples_per_pixel;
	float_v4 sample_scale = (float_v4){{pixels_samples_scale, pixels_samples_scale, pixels_samples_scale, pixels_samples_scale}};
	for (size_t i = 0; i < image_width * image_height; i++) {
		float_v4 color = color_data[i];

		color = float_v4_vec_scale(color, sample_scale);
		
		color.e[0] = linear_to_gamma(color.e[0]);
		color.e[1] = linear_to_gamma(color.e[1]);
		color.e[2] = linear_to_gamma(color.e[2]);
		color.e[3] = linear_to_gamma(color.e[3]);

		Interval intensity = {0.0f, 0.999f};
		color.e[0] = 256.0f * Interval_clamp(intensity, color.e[0]);
		color.e[1] = 256.0f * Interval_clamp(intensity, color.e[1]);
		color.e[2] = 256.0f * Interval_clamp(intensity, color.e[2]);
		color.e[3] = 256.0f * Interval_clamp(intensity, color.e[3]); // no transparency

		img_data[i] = float_v4_to_uint8_v4(color);
	}
	puts("\x1b[1K\rDone!");

	free(color_data);

	return img;
}

