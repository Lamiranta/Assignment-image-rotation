#include "bmpfile.h"

int main( int argc, char** argv )
{
	if ( argc != 3 )
	{
		error_message( "Incorrect file name!" );
		return 1;
	}

	bmp_transform( argv[1], argv[2] );
	
	return 0;
}
