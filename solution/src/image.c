#include "image.h"

void set_pixel( struct image* image, size_t width, size_t height, struct pixel pixel )
{
	image->data[ height * image->width + width ] = pixel;
}

struct pixel get_pixel( struct image const* image, size_t width, size_t height )
{
	return ( image->data )[ height * image->width + width ];
}

struct image load_image( uint64_t width, uint64_t height )
{
	struct pixel* data = malloc( width * height * sizeof( struct pixel ) );

	struct image temp = {
		.width = width,
		.height = height,
		.data = data
	};

	return temp;
}

void forget_image( struct image* image )
{
	free( image->data );
	image->width = 0;
	image->height = 0;
}

bool check_image( struct image* image )
{
	return (image != NULL) && (image->data != NULL) && (image->height > 0) && (image->width > 0);
}
