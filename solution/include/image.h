#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <malloc.h>

#pragma pack ( push, 1 )

struct pixel
{
	uint8_t r, g, b;
};

#pragma pack ( pop )

struct image
{
	uint64_t		width, height;
	struct pixel*		data;
};

void set_pixel( 		struct image* image, size_t width, size_t height, struct pixel pixel 	);
struct pixel get_pixel(		struct image const* image, size_t width, size_t height			);

struct image load_image( 	uint64_t width, uint64_t height						);
void forget_image( 		struct image* image							);
bool check_image( 		struct image* image 							);

#endif
