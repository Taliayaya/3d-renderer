# ifndef PERSPECTIVE_H
# define PERSPECTIVE_H

# include <math.h>

# include "math/matrix.h"

Mat4 *perspective(float fov, float aspect, float near, float far);

# endif
