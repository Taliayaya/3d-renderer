# include "../../include/math/matrix.h"

Mat4 *new_mat4()
{
    return calloc(1, sizeof(Mat4));
}

Mat4 *new_mat4_id(float eigen)
{
    Mat4 *mat = new_mat4();
    for (int i = 0; i < 4; ++i)
    {
        mat->arr[i][i] = eigen;
    }
    return mat;
}


void print_mat4(Mat4 *mat)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            printf("%f ", mat->arr[i][j]);
        printf("\n");
    }
}

int main()
{
    Mat4 *m = new_mat4_id(1);
    print_mat4(m);
    printf("size: %zu\n", sizeof(Mat4));
}
