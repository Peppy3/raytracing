#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vec.h>

#include "image.h"

#define PACKED __attribute__((__packed__))
#define ALIGNED __attribute__((aligned))

struct Image {
	uint32_t width;
	uint32_t height;
	uint8_v4 data[] ALIGNED;
};

static inline size_t Image_data_size(const struct Image *img) {
	return img->width * img->height * (sizeof *img->data);
}

struct Image *Image_new(uint32_t width, uint32_t height) {
	struct Image *img = NULL;
	
	img = malloc(sizeof *img + width * height * (sizeof *img->data));
	if (img == NULL) {
		return NULL;
	}

	*img = (struct Image) {
		.width = width,
		.height = height
	};

	return img;
}

uint32_t Image_get_width(const struct Image *img) { return img->width; }
uint32_t Image_get_height(const struct Image *img) { return img->height; }
uint8_v4 *Image_get_data(struct Image *img) { return img->data; }

struct Image *Image_copy(const struct Image *img) {
	struct Image *new = NULL;
	size_t size = sizeof *img + img->width * img->height * sizeof *img->data;
	new = malloc(size);
	if (new == NULL) {
		return NULL;
	}
	
	return memcpy(new, img, size);
}

// Flips an image upside down
struct Image *Image_flip_top_bottom(struct Image *img) {
	uint8_v4 tmp = {0};
	size_t array_size = (img->width * img->height);

	uint8_v4 *img_data = img->data;
	
	for (size_t i = 0; i < (array_size / 2); i++) {
		tmp = img_data[i];
		img_data[i] = img_data[array_size - i];
		img_data[array_size - i] = tmp;
	}

	return img;
}

// Flips an image horizontally
struct Image *Image_flip_left_right(struct Image *img) {
	uint8_v4 tmp = {0};
	uint32_t image_width = img->width;

	for (size_t j = 0; j < img->height; j++) {
		uint8_v4 *row = &(img->data[j * image_width]);
		for (size_t i = 0; i < (image_width / 2); i++) {
			tmp = row[i];
			row[i] = row[image_width - i];
			row[image_width - i] = tmp;
		}
	}

	return img;
}

int Image_save_as_ppm(const struct Image *img, const char *filename) {
	FILE *fp = NULL;

	fp = fopen(filename, "w");
	if (fp == NULL) {
		perror(filename);
		return 1;
	}
	
	fprintf(fp, 
			"P3\n"
			"%u %u\n"
			"255\n",
			img->width, img->height);

	for (size_t i = 0; i < img->width * img->height; i++) {
		uint8_v4 vec = img->data[i];
		fprintf(fp, "%u %u %u\n", 
				vec.x, vec.y, vec.z
			   );
	}

	fclose(fp);

	return 0;
}

struct PACKED BitmapImageHeader {
	char magic[2];
	uint32_t file_size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t array_offset;
};

struct BitmapInfoHeader {
	uint32_t header_size; // 40
	int32_t width;
	int32_t height;
	uint16_t num_color_planes; // must be 1
	uint16_t bits_per_pixel;
	uint32_t compression_method; // 0 for uncompressed
	uint32_t raw_image_size;
	int32_t width_ppm; // pixel per metre
	int32_t height_ppm;
	uint32_t num_color; // number of colors in color palete
	uint32_t important_colors;
};

int Image_save_as_bmp(const struct Image *img, const char *filename) {
	FILE *fp = NULL;
	uint8_v3 *bmp_row = NULL;

	struct BitmapImageHeader header = {
		.magic = {'B', 'M'},
		.file_size = 0,
		.reserved1 = 0,
		.reserved2 = 0,
		.array_offset = 0
	};

	struct BitmapInfoHeader info_header = {
		.header_size = 40,
		.width = img->width,
		.height = img->height,
		.num_color_planes = 1,
		.bits_per_pixel = 24,
		.compression_method = 0,
		.raw_image_size = 0,
		.width_ppm = 0,
		.height_ppm = 0,
		.num_color = 0,
		.important_colors = 0
	};

	size_t bmp_row_size = ((img->width * sizeof(uint8_v3)) + sizeof(uint8_v3)) & (-4);
	size_t raw_image_size = bmp_row_size * img->height; // raw bmp image size with padding
	info_header.raw_image_size = raw_image_size;
	header.array_offset = sizeof(struct BitmapImageHeader) + sizeof(struct BitmapInfoHeader);
	header.file_size = header.array_offset + raw_image_size;

	bmp_row = malloc(bmp_row_size);
	if (bmp_row == NULL) {
		return 1;
	}

	fp = fopen(filename, "wb");
	if (fp == NULL) {
		free(bmp_row);
		perror(filename);
		return 1;
	}


	fwrite(&header, sizeof header, 1, fp);
	fwrite(&info_header, sizeof info_header, 1, fp);

	for (long int i = img->height - 1; i >= 0; i--) {

		for (size_t j = 0; j < img->width; j++) {
			size_t idx = i * img->width + j;
			uint8_v4 tmp = img->data[idx];
			bmp_row[j] = (uint8_v3){tmp.z, tmp.y, tmp.x};
		}
		fwrite(bmp_row, bmp_row_size, 1, fp);
	}
	
	fclose(fp);
	free(bmp_row);

	return 0;
}

