# include "../../include/math/vec4.h"

Vec4 *new_null_vec4(void)
{
    return calloc(1, sizeof(Vec4));
}

Vec4 *new_vec4(float x, float y, float z, float w)
{
    Vec4 *vec4 = new_null_vec4();
    vec4->x = x;
    vec4->y = y;
    vec4->z = z;
    vec4->w = w;
    return vec4;
}

void print_vec4(Vec4 *vec4)
{
    printf("(%f, %f, %f, %f)\n", vec4->x, vec4->y, vec4->z, vec4->w);
}
