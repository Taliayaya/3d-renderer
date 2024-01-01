# ifndef MATRIX_H
# define MATRIX_H

# define PI 3.14159265359 
# define TO_RAD(x) (x) * (PI/180)
# define TO_DEG(x) (x) * (180/PI)

# include <stdlib.h>
# include <stdio.h>
# include <math.h>

# include "vec4.h"
# include "vec3.h"

typedef struct Mat4
{
    float arr[4][4];
} Mat4;

// ==== Constructors ====

Mat4 *new_mat4(void);
Mat4 *new_mat4_id(float eigen);

void free_mat4(Mat4 *mat);

// ==== Basic Operations ====

Mat4 *mat4_dot(Mat4 *a, Mat4 *b);

void print_mat4(Mat4 *mat);

// === Transformations ===

Mat4 *mat4_translate(Mat4 *self, Vec3 *trans);
Mat4 *mat4_rotate(Mat4 *self, float angle, Vec3 *axis);
Mat4 *mat4_scale(Mat4 *self, Vec3 *trans);

#endif
