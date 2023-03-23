#ifndef BMPFILEIO_H_
#define BMPFILEIO_H_

#include "image.h"
#include <stdio.h>

FILE* file_read_only( 	const char* path 	);
FILE* file_write_only( 	const char* path 	);

void close_file( 	FILE* file 		);

void error_message( 	const char* message     );

#endif
