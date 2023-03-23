#include "bmpfileio.h"

FILE* file_read_only( const char* path )
{
	FILE* input = fopen(path, "rb");
	if ( !input )
	{
		error_message( "Can't read from non-existed file!" );
		return NULL;
	}
	return input;
}

FILE* file_write_only( const char* path )
{
	FILE* output = fopen(path, "wb");
	if ( !output )
	{
		error_message( "Can't write in non-existed file!" );
		return NULL;
	}
	return output;
}

void close_file( FILE* file )
{
	if ( file )
	{
		int temp = fclose( file );
		if ( temp != 0 )
		{
			error_message( "Trying to close a non-existed file! Please, correct path names!" );
		}
	}
	else { error_message( "Programm error occured: file object doesn't exist!" ); }
}

void error_message( const char* message )
{
	fprintf( stderr, "%s\n", message );
}
