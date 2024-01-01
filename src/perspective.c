# include "../include/perspective.h"

Mat4 *perspective(float FoV, float aspect, float near, float far)
{
    Mat4 *mat = new_mat4();
    // first column
    mat->arr[0][3] = 0;
    mat->arr[1][3] = 0;
    mat->arr[2][3] = -1;
    mat->arr[3][3] = 0;


    // last column
    mat->arr[0][3] = 0;
    mat->arr[1][3] = 0;
    mat->arr[2][3] = -1;
    mat->arr[3][3] = 0;
}
