# ifndef CAMERA_H
# define CAMERA_H

# include <err.h>

# include "../include/math/vec3.h"
# include "../include/math/matrix.h"
# include "../include/transform.h"

typedef struct Camera
{
    Vec3 target;
    Transform transform;
} Camera;

Camera *new_camera(Vec3 *position, Vec3 *target, Vec3 *up);

void free_camera(Camera *camera);

Mat4 *camera_look_at(Camera *camera);

# endif
