#include <stddef.h>

#include <vec.h>

void Matrix_mul(size_t h, size_t w, 
		const float lhs[h][w], const float rhs[h][w], float res[h][w]) {
	for (size_t i = 0; i < h; i++) {
		for (size_t j = 0; j < w; j++) {
			res[i][j] = 0.0;
		}
	}

	for (size_t i = 0; i < h; i++) {
		for (size_t j = 0; j < w; j++) {
			float acc = 0.0;
			for (size_t k = 0; k < h; k++) {
				acc += lhs[i][k] * rhs[k][j];
			}
			res[i][j] = acc;
		}
	}
}

float_v3 Matrix_float_v3_mul(const float lhs[3][3], const float_v3 rhs) {
	float_v3 res = {0};
	for (size_t i = 0; i < 3; i++) {
		res.x += lhs[0][i] * rhs.x;
		res.y += lhs[1][i] * rhs.y;
		res.z += lhs[2][i] * rhs.z;
	}
	return res;
}

