# include "../../include/math/vec3.h"

Vec3 new_empty_vec3(void)
{
    return (Vec3){0, 0, 0};
}

Vec3 new_vec3(float x, float y, float z)
{
    return (Vec3){x, y, z};
}

void print_vec3(Vec3 *a)
{
    printf("{%f, %f, %f}\n",
            a->x, a->y, a->z);
}

float vec3_magnitude(Vec3 *vec)
{
    return sqrt(vec->x * vec->x +
                vec->y * vec->y +
                vec->z * vec->z);
}

void vec3_negate(Vec3 *vec)
{
    vec->x = -vec->x;
    vec->y = -vec->y;
    vec->z = -vec->z;
}

Vec3 vec3_negated(Vec3 *vec)
{
    return (Vec3){-vec->x,
                  -vec->y,
                  -vec->z};
}
void vec3_normalize(Vec3 *vec)
{
    float magn = vec3_magnitude(vec);
    vec->x /= magn;
    vec->y /= magn;
    vec->z /= magn;
}

Vec3 vec3_normalized(Vec3 *vec)
{
    float magn = vec3_magnitude(vec);
    return (Vec3){vec->x / magn,
                  vec->y / magn,
                  vec->z / magn};

}

Vec3 vec3_mul(Vec3 *a, float lambda)
{
    return (Vec3){a->x * lambda,
                  a->y * lambda,
                  a->z * lambda};
}

Vec3 vec3_add(Vec3 *a, Vec3 *b)
{
    return (Vec3){a->x + b->x,
                  a->y + b->y,
                  a->z + b->z};
}

Vec3 vec3_sub(Vec3 *a, Vec3 *b)
{
    return (Vec3){a->x - b->x,
                  a->y - b->y,
                  a->z - b->z};
}

Vec3 vec3_cross(Vec3 *a, Vec3 *b)
{
    Vec3 cross = new_empty_vec3();
    cross.x = a->y * b->z - a->z * b->y;
    cross.y = a->z * b->x - a->x * b->z;
    cross.z = a->x * b->y - a->y * b->x;
    return cross;
}

float vec3_dot(Vec3 *a, Vec3 *b)
{
    return a->x * b->x +
           a->y * b->y +
           a->z * b->z;
}
