# ifndef VEC3_H
# define VEC3_H

# include <math.h>

typedef struct Vec3
{
    float x;
    float y;
    float z;
} Vec3;

Vec3 new_empty_vec3(void);
Vec3 new_vec3(float x, float y, float z);

void print_vec3(Vec3 *a);


float vec3_magnitude(Vec3 *vec);

void vec3_negate(Vec3 *vec);
Vec3 vec3_negated(Vec3 *vec);



void vec3_normalize(Vec3 *vec); // in place
Vec3 vec3_normalized(Vec3 *vec);

Vec3 vec3_add(Vec3 *a, Vec3 *b);
Vec3 vec3_sub(Vec3 *a, Vec3 *b);
Vec3 vec3_mul(Vec3 *a, float lambda);


Vec3 vec3_cross(Vec3 *a, Vec3 *b);
float vec3_dot(Vec3 *a, Vec3 *b);




# endif
