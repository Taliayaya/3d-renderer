# ifndef MATRIX_H
# define MATRIX_H

# include <stdlib.h>
# include <stdio.h>

typedef struct Mat4
{
    float arr[4][4];
} Mat4;

Mat4 *new_mat4();
Mat4 *new_mat4_id(float eigen);

void print_mat4(Mat4 *mat);

#endif
