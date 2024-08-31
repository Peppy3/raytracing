#ifndef IMAGE_H
#define IMAGE_H

#include <stddef.h>
#include <stdint.h>

#include <vec.h>
#include <camera.h>

struct Image;

// Image constructor
// only colorspace option is 24-bit RGB (8-bit per channel)
// free with stdlib free
struct Image *Image_new(uint32_t width, uint32_t height);

uint32_t Image_get_width(const struct Image *img);
uint32_t Image_get_height(const struct Image *img);
uint8_v4 *Image_get_data(struct Image *img);

// Makes a copy of an image returned image should be freed seperately
struct Image *Image_copy(const struct Image *img);

// Flips an image upside down
struct Image *Image_flip_top_bottom(struct Image *img);

// saves image to a file
// CameraOptions struct is optionaly NULL
int Image_save_as_ppm(const struct Image *img, const char *filename, struct CameraOptions *opts);
int Image_save_as_bmp(const struct Image *img, const char *filename);

#endif /* IMAGE_H */

