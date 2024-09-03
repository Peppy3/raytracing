#ifndef VEC_H
#define VEC_H

#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>
#include <math.h>

#include <def.h>

typedef union uint8_v3 {
	struct {uint8_t x, y, z;};
	uint8_t e[3];
} uint8_v3;

typedef union uint8_v4 {
	struct {uint8_t x, y, z, w;};
	uint8_t e[4];
} uint8_v4;

typedef union float_v2 {
	struct {float x, y;};
	struct {float u, v;};
	float e[2];
} float_v2;

typedef union float_v3 {
	struct {float x, y, z;};
	float e[3];
} float_v3;

typedef float_v3 point3;

typedef union float_v4 {
	struct {float x, y, z, w;};
	float e[4];
} float_v4;


inline float_v3 float_v3_scale(const float_v3 vec, float scalar) {
	return (float_v3) {{vec.x * scalar, vec.y * scalar, vec.z * scalar}};
}

inline float_v3 float_v3_vec_scale(const float_v3 vec, float_v3 scalar) {
	return (float_v3) {{vec.x * scalar.x, vec.y * scalar.y, vec.z * scalar.z}};
}

inline float_v3 float_v3_div(const float_v3 vec, float scalar) {
	if (scalar == 0.0f) {
		return (float_v3){{0.0f, 0.0f, 0.0}};
	}
	return float_v3_scale(vec, 1/scalar);
}

inline float_v3 float_v3_add(const float_v3 lhs, const float_v3 rhs) {
	return (float_v3) {{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z}};
}

inline float_v3 float_v3_sub(const float_v3 lhs, const float_v3 rhs) {
	return (float_v3) {{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z}};
}

inline float_v3 float_v3_neg(const float_v3 v) {
	return (float_v3) {{-v.x, -v.y, -v.z}};
}

inline float_v3 float_v3_mul(const float_v3 lhs,const  float_v3 rhs) {
	return (float_v3) {{lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z}};
}

inline float float_v3_dot(const float_v3 lhs, const float_v3 rhs) {
	return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

inline float_v3 float_v3_cross(const float_v3 lhs, const float_v3 rhs) {
	return (float_v3) {{
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x
	}};
}

inline float_v3 float_v3_square(const float_v3 vec) {
	return (float_v3) {{vec.x * vec.x, vec.y * vec.y, vec.z * vec.z}};
}

inline float float_v3_length_squared(const float_v3 vec) {
	return float_v3_dot(vec, vec);
}

inline float float_v3_length(const float_v3 vec) {
	return sqrtf(float_v3_dot(vec, vec));
}

inline float_v3 float_v3_unit_vector(const float_v3 vec) {
	return float_v3_div(vec, float_v3_length(vec));
}
#define float_v3_norm(vec) float_v3_unit_vector(vec)

float_v4 euler_to_quaternion(float roll, float pitch, float yaw);

// both should be unit length
float_v3 float_v3_rotate_by_quaternion(const float_v3 base, const float_v4 q);

// base should be unit length
inline float_v3 float_v3_rotate_euler(float roll, float pitch, float yaw, const float_v3 base) {
	float_v4 q = euler_to_quaternion(roll, pitch, yaw);
	
	return float_v3_rotate_by_quaternion(base, q);
}

static inline float_v3 float_v3_rand(void) {
	return (float_v3){{random_float(), random_float(), random_float()}}; 
}

static inline float_v3 float_v3_rand_min_max(float min, float max) {
	return (float_v3){{
		random_float_min_max(min, max),
		random_float_min_max(min, max),
		random_float_min_max(min, max)
	}};
}

// returns true if close to zero in all directions
inline bool float_v3_near_zero(const float_v3 vec) {
	const float s = 1e-8;
	return (fabsf(vec.x) < s) && (fabsf(vec.y) < s) && (fabsf(vec.z) < s);
}

inline float_v3 float_v3_reflect(const float_v3 v, const float_v3 n) {
	return float_v3_sub(v, 
			float_v3_scale(n,
				float_v3_dot(v, n) * 2.0f
				)
			);
}

inline float_v3 float_v3_refract(const float_v3 uv, const float_v3 n, 
		float eati_over_etat) {
	float cos_theta = fminf(float_v3_dot(float_v3_neg(uv), n), 1.0);
	float_v3 r_out_prep = float_v3_scale(
			float_v3_add(uv, float_v3_scale(n, cos_theta)),
			eati_over_etat
			);
	float_v3 r_out_parallel = float_v3_scale(n,
		-sqrtf(fabsf(1.0f - float_v3_length_squared(r_out_prep))));
	return float_v3_add(r_out_prep, r_out_parallel);
}

inline int float_v3_print(const float_v3 vec) {
	return printf("(%.3f, %.3f, %.3f)\n",
			vec.x, vec.y, vec.z);
}

// generates a random position inside a disk with a given radius
static inline float_v3 float_v3_rand_in_unit_disk(void) {
	while (true) {
		float_v3 p = {{
			random_float_min_max(-1.0f, 1.0f), 
			random_float_min_max(-1.0f, 1.0f), 
			0.0f
		}};
		if (float_v3_length_squared(p) < 1.0f) {
			return p;
		}
	}
}

// generates a random vector inside a sphere with a given radius
static inline float_v3 float_v3_rand_in_unit_sphere(void) {
	while (true) {
		float_v3 p = float_v3_rand_min_max(-1.0f, 1.0f);
		if (float_v3_length_squared(p) < 1.0f) {
			return p;
		}
	}
}

static inline float_v3 float_v3_rand_unit_vector(void) {
	return float_v3_unit_vector(float_v3_rand_in_unit_sphere());
}

static inline float_v3 float_v3_rand_on_hemisphere(const float_v3 normal) {
	float_v3 on_unit_sphere = float_v3_rand_unit_vector();
	if (float_v3_dot(on_unit_sphere, normal) > 0.0f) {
		return on_unit_sphere;
	}
	return float_v3_neg(on_unit_sphere);
}

inline uint8_v3 float_v3_to_uint8_v3(const float_v3 vec) {
	return (uint8_v3) {{(uint8_t)vec.x, (uint8_t)vec.y, (uint8_t)vec.z}};
}


inline float_v4 float_v4_scale(const float_v4 vec, float scalar) {
	return (float_v4) {{vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar}};
}

inline float_v4 float_v4_vec_scale(const float_v4 vec, float_v4 scalar) {
	return (float_v4) {{vec.x * scalar.x, vec.y * scalar.y, vec.z * scalar.z, vec.w * scalar.w}};
}

inline float_v4 float_v4_div(const float_v4 vec, float scalar) {
	if (scalar == 0.0f) {
		return (float_v4){{0.0, 0.0, 0.0, 0.0}};
	}
	return float_v4_scale(vec, 1/scalar);
}

inline float_v4 float_v4_add(const float_v4 lhs, const float_v4 rhs) {
	return (float_v4) {{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.y + rhs.y}};
}

inline float_v4 float_v4_sub(const float_v4 lhs, const float_v4 rhs) {
	return (float_v4) {{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w}};
}

inline float float_v4_dot(const float_v4 lhs, const float_v4 rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

inline float float_v4_length_squared(const float_v4 vec) {
	return float_v4_dot(vec, vec);
}

inline float float_v4_length(const float_v4 vec) {
	return sqrtf(float_v4_dot(vec, vec));
}

inline float_v4 float_v4_unit_vector(const float_v4 vec) {
	return float_v4_div(vec, float_v4_length(vec));
}
#define float_v4_norm(vec) float_v4_unit_vector(vec)

inline int float_v4_print(float_v4 vec) {
	return printf("(%f, %f, %f, %f)\n",
		vec.x, vec.y, vec.z, vec.w);
}

// Hamilton product
inline float_v4 quaternion_multiply(const float_v4 lhs, const float_v4 rhs) {
	return (float_v4) {{
		.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
		.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
		.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
		.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
	}};
}

inline uint8_v4 float_v4_to_uint8_v4(const float_v4 vec) {
	return (uint8_v4) {{(uint8_t)vec.x, (uint8_t)vec.y, (uint8_t)vec.z, (uint8_t)vec.w}};
}

#endif /* VEC_H */

