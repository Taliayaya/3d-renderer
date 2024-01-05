# include "../include/perspective.h"

Mat4 perspective(float fov, float aspect, float near, float far)
{
    if (fov <= 0 || aspect == 0)
        errx(EXIT_FAILURE, "bad fov / aspect for perspective matrix");

    float frustum_depth = far - near;
    float one_over_depth = 1/ frustum_depth;
    Mat4 mat = new_mat4();

    // right handed
    mat.arr[1][1] = 1 / tan(fov / 2);
    mat.arr[0][0] = mat.arr[1][1] / aspect;
    mat.arr[2][2] = - (far + near) * one_over_depth;
    mat.arr[2][3] = -(2 * far * near) * one_over_depth;
    mat.arr[3][2] = -1;
    mat.arr[3][3] = 0;

    return mat;
}
