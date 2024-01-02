# ifndef TRANSFORM_H
# define TRANSFORM_H

# include "../include/math/vec3.h"

typedef struct Transform
{
    Vec3 up;
    Vec3 right;
    Vec3 forward;

    Vec3 position;
} Transform;

# endif
