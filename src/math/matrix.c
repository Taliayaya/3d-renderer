# include "../../include/math/matrix.h"

Mat4 *new_mat4(void)
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

void free_mat4(Mat4 *mat)
{
    free(mat);
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

Mat4 *mat4_translate(Mat4 *self, Vec3 *trans)
{
    Mat4 *translation_mat = new_mat4_id(1.f);
    translation_mat->arr[0][3] += trans->x;
    translation_mat->arr[1][3] += trans->y;
    translation_mat->arr[2][3] += trans->z;

    Mat4 *transf_mat = mat4_dot(self, translation_mat);
    free_mat4(translation_mat);
    return transf_mat;
}

Mat4 *mat4_scale(Mat4 *self, Vec3 *trans)
{
    Mat4 *mat = new_mat4();
    mat->arr[0][0] = trans->x;
    mat->arr[1][1] = trans->y;
    mat->arr[2][2] = trans->z;
    mat->arr[3][3] = 1;

    Mat4 *transf_mat = mat4_dot(self, mat);
    free_mat4(mat);
    return transf_mat;
}

Mat4 *mat4_rotate(Mat4 *self, float angle, Vec3 *axis)
{
    Mat4 *mat = new_mat4();
    float x, y, z;
    float cos_a = cos(angle);
    float sin_a = sin(angle);
    vec3_normalize(axis);
    x = axis->x;
    y = axis->y;
    z = axis->z;
    {
        float x_sq = x * x;
        // first row
        mat->arr[0][0] = x_sq + (1 - x_sq) * cos_a; 
        mat->arr[0][1] = x * y * (1 - cos_a) - z * sin_a;
        mat->arr[0][2] = x * z * (1 - cos_a) + y * sin_a;
    }
    {
        float y_sq = y * y;
        // second row
        mat->arr[1][0] = x * y * (1 - cos_a) + z * sin_a; 
        mat->arr[1][1] = y_sq + (1 - y_sq) * cos_a;
        mat->arr[1][2] = y * z * (1 - cos_a) - x * sin_a;
    }
    {
        float z_sq = z * z;
        // third row
        mat->arr[2][0] = x * z * (1 - cos_a) - y * sin_a; 
        mat->arr[2][1] = y * z * (1 - cos_a) + x * sin_a;
        mat->arr[2][2] = z_sq + (1 - z_sq) * cos_a;
    }
    mat->arr[3][3] = 1;

    Mat4 *transf_mat = mat4_dot(self, mat);
    free_mat4(mat);
    return transf_mat;
}

Mat4 *mat4_dot(Mat4 *a, Mat4 *b)
{
    Mat4 *res = new_mat4();
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 4; ++k)
                res->arr[i][j] += a->arr[i][k] * b->arr[k][j];
    return res;
}

// a variant invert specialized for transform matrices
// do not use if the matrix isnt in a format
// [ux vx wx tx]
// [uy vy wy ty]
// [uz vz wz tz]
// [ 0  0  0  1]

Mat4 *mat4_transform_invert(Mat4 *a)
{
    Mat4 *inv = new_mat4_id(1.f);
    // create vectors from columns
    Vec3 u = {a->arr[0][0], a->arr[1][0], a->arr[2][0]};
    Vec3 v = {a->arr[0][1], a->arr[1][1], a->arr[2][1]};
    Vec3 w = {a->arr[0][2], a->arr[1][2], a->arr[2][2]};
    Vec3 t = {a->arr[0][3], a->arr[1][3], a->arr[2][3]};

    // first row
    inv->arr[0][0] = u.x;
    inv->arr[0][1] = u.y;
    inv->arr[0][2] = u.z;
    inv->arr[0][3] = -vec3_dot(&u, &t);

    // second row
    inv->arr[1][0] = v.x;
    inv->arr[1][1] = v.y;
    inv->arr[1][2] = v.z;
    inv->arr[1][3] = -vec3_dot(&v, &t);

    // third row
    inv->arr[2][0] = w.x;
    inv->arr[2][1] = w.y;
    inv->arr[2][2] = w.z;
    inv->arr[2][3] = -vec3_dot(&w, &t);

    // last row is unchanged (0 0 0 1)

    return inv;
}

Vec4 *mat4_dot_vec4(Mat4 *a, Vec4 *b)
{
    Vec4 *res = new_null_vec4();
    float vec[4] = {0,};
    for (int i = 0; i < 4; ++i)
    {
        vec[i] += a->arr[i][0] * b->x +
                  a->arr[i][1] * b->y +
                  a->arr[i][2] * b->z +
                  a->arr[i][3] * b->w;
    }
    res->x = vec[0];
    res->y = vec[1];
    res->z = vec[2];
    res->w = vec[3];

    return res;
}

void mat4_transpose(Mat4 *a)
{
    for (int i = 0; i < 4; ++i)
        for (int j = i; j < 4; ++j)
        {
            float tmp = a->arr[i][j];
            a->arr[i][j] = a->arr[j][i];
            a->arr[j][i] = tmp;
        }
}

void mat4_negate(Mat4 *a)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            a->arr[i][j] = -a->arr[i][j];
}

void mat4_transpose_negate(Mat4 *a)
{
    for (int i = 0; i < 4; ++i)
        for (int j = i; j < 4; ++j)
        {
            float tmp = a->arr[i][j];
            a->arr[i][j] = -a->arr[j][i];
            a->arr[j][i] = -tmp;
        }
}
