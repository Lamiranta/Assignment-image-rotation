#ifndef ROTATION_H_
#define ROTATION_H_

#include "image.h"
#include <string.h>

typedef struct image (*transformation)( struct image const*, struct image               );

struct image transform(                 struct image const* image, transformation T     );

struct image rotate_90_counter(         struct image const* image, struct image temp    );

#endif
