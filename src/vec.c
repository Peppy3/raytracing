#include <stddef.h>

#include <stdio.h>
#include <math.h>

#include <def.h>
#include <matrix.h>
#include <vec.h>

float_v4 euler_to_quaternion(float pitch, float yaw, float roll) {
	float_v3 rads = {{
		degrees_to_radians(pitch / 2),
		degrees_to_radians(yaw / 2),
		degrees_to_radians(roll / 2),
	}};

	float_v3 s = (float_v3) {{
		sinf(rads.x),
		sinf(rads.y),
		sinf(rads.z),
	}};
	
	float_v3 c = (float_v3) {{
		cosf(rads.x),
		cosf(rads.y),
		cosf(rads.z),
	}};
	
	return float_v4_unit_vector((float_v4) {{
		.w = c.x * c.y * c.z + s.x * s.y * s.z,
		.x = s.x * c.y * c.z - c.x * s.y * s.z,
		.y = c.x * s.y * c.z + s.x * c.y * s.z,
		.z = c.x * c.y * s.z - s.x * s.y * c.z,
	}});
}

float_v3 float_v3_rotate_by_quaternion(const float_v3 base, const float_v4 q) {
	// To rotate a vector by a quaternion, use: (q * forward * q^-1)
	
	float_v4 q_conjugate = (float_v4){{-q.x, -q.y, -q.z, q.w}};
	float_v4 rotated = quaternion_multiply(
			quaternion_multiply(q, (float_v4){{base.x, base.y, base.z, 0.0f}}),
			q_conjugate
			);

	return (float_v3){{rotated.x, rotated.y, rotated.z}};
}


