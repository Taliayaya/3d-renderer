# ifndef VEC4_H
# define VEC4_H

# include <stdlib.h>
# include <stdio.h>

typedef struct Vec4
{
    float x;
    float y;
    float z;
    float w;
} Vec4;

// === Constructors ===

Vec4 *new_null_vec4(void);
Vec4 *new_vec4(float x, float y, float z, float w);

// === Basic Operations ===

void print_vec4(Vec4 *vec4);

# endif
