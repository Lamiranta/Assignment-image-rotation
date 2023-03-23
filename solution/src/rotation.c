#include "rotation.h"

struct image transform( struct image const* image, transformation T )
{
	struct image temp = {
		.width = image->height,
		.height = image->width,
		.data = malloc( image->height * image->width * sizeof( struct pixel ) )
	};

	return (*T)( image, temp );
}

struct image rotate_90_counter( struct image const* image, struct image temp )
{	
	for ( uint64_t i = 0; i < temp.width; ++i )
	{
		for ( uint64_t j = 0; j < temp.height; ++j )
		{
			set_pixel( &temp, i, j, get_pixel( image, j, temp.width - i - 1 ) );
		}
	}

	return temp;
}
