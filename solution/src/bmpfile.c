#include "bmpfile.h"

#pragma pack( push, 1 )

struct bmp_header
{
	uint16_t bfType;
	uint32_t bfileSize;
	uint32_t bfReserved;
	uint32_t bOffBits;
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};

#pragma pack( pop )

struct bmp_file
{
	struct bmp_header	header;
	struct image		image;
};

static bool check_header( struct bmp_header* header )
{
	return (header->bfType == 0x4d42) && (header->biBitCount == 24);
}

static bool check_file( struct bmp_file* bmp_file )
{
	return check_header( &(bmp_file->header) ) && check_image( &(bmp_file->image) );
}

static uint8_t get_padding( struct bmp_header* header )
{
	return header->biWidth % 4;
}

static struct bmp_file from_bmp( FILE* in )
{
	struct bmp_file temp = {0};
	
	const size_t flag = fread( &(temp.header), sizeof( struct bmp_header ), 1, in );
	
	if( flag != 1 || !check_header( &(temp.header) ) )
	{
		error_message( "Error in loading header!" );
		return (struct bmp_file) {0};
	}
	
	temp.image = load_image( temp.header.biWidth, temp.header.biHeight );
	
	if( !check_image( &(temp.image) ) )
	{
		error_message( "Error in loading image!" );
		return (struct bmp_file) {0};
	}
	
	const uint8_t padding = get_padding( &(temp.header) );
	uint64_t num_elems = 0;

	for( uint64_t i = 0; i < temp.image.height; ++i )
	{
		num_elems = fread( &( temp.image.data[ i * temp.image.width ] ), sizeof( struct pixel ), temp.image.width, in );
		if ( num_elems != temp.image.width )
		{
			error_message( "Loaded number of pixels differs from actual number! Please, check image correctness!" );
			return (struct bmp_file) {0};
		}
		fseek( in, padding, SEEK_CUR );
	}
	
	return temp;
}

static bool to_bmp( FILE* out, struct bmp_file* bmp_file )
{
	if ( out == NULL || bmp_file->image.data == NULL )
	{
		return false;
	}

	bmp_file->header.biWidth = bmp_file->image.width;
	bmp_file->header.biHeight = bmp_file->image.height;

	const uint8_t padding = get_padding( &(bmp_file->header) );
	const uint64_t pointer = 0;
	struct pixel* ptr = bmp_file->image.data;

	bmp_file->header.biSizeImage = bmp_file->image.height * bmp_file->image.width * sizeof( struct pixel ) + padding * bmp_file->image.height;
	bmp_file->header.bfileSize = bmp_file->image.height * bmp_file->image.width * sizeof( struct pixel ) + 
				      bmp_file->image.height * padding + sizeof( struct bmp_header );
	
	uint64_t temp = 0;
	temp = fwrite( &(bmp_file->header), sizeof( struct bmp_header ), 1, out );
	if ( temp != 1 )
	{
		error_message( "Error occured while writing bmp header!" );
		return false;
	}
	
	for ( uint64_t i = 0; i < bmp_file->image.height; ++i )
	{
		temp = fwrite( ptr, 1, bmp_file->image.width * sizeof( struct pixel ), out );
		if ( temp != bmp_file->image.width * sizeof( struct pixel ) )
		{
			error_message( "Written number of pixels differs from actual number! Please, check file format!" );
			return false;
		}
		temp = fwrite( &pointer, 1, padding, out );
		if ( temp != padding )
		{
			error_message( "Error occured while writing bmp padding!" );
			return false;
		}
		ptr += bmp_file->image.width;
	}
	
	close_file( out );
	return true;
}

static struct bmp_file wrapper_input( const char* path )
{
	FILE* input = file_read_only( path );
	if ( input == NULL )
	{
		return (struct bmp_file) {0};
	}
	struct bmp_file temp = from_bmp( input );
	if ( !check_file( &temp ) )
	{
		error_message( "File was read incorrectly!" );
		close_file( input );
		return (struct bmp_file) {0};
	}
	return temp;
}

static bool wrapper_output( const char* path, struct bmp_file* bmp_file )
{
	FILE* output = file_write_only( path );
	if ( output == NULL )
	{
		return false;
	}
	bool check = to_bmp( output, bmp_file );
	if ( !check )
	{
		error_message( "Error occured while saving image!" );
		close_file( output );
		return false;
	}
	return true;
}

void bmp_transform( const char* input, const char* output )
{
	struct bmp_file bmp = wrapper_input( input );
	
	struct image temp = bmp.image;
	bmp.image = transform( &temp, &rotate_90_counter );
	forget_image( &temp );
	
	wrapper_output( output, &bmp );
	
	forget_image( &(bmp.image) );
}
