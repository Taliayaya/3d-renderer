# ifndef SHADER_H
# define SHADER_H

# include "../include/glad/glad.h"

# include <string.h>
# include <stdio.h>
# include <err.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdbool.h>

typedef struct Shader
{
    unsigned int ID;
} Shader;

Shader *new_shader(const char *vertexPath, const char *fragmentPath);

void free_shader(Shader *self);

void shader_use(Shader *self);

void shader_set_bool(Shader *self, const char *name, bool value);
void shader_set_int(Shader *self, const char *name, int value);
void shader_set_float(Shader *self, const char *name, float value);
void shader_set_vec4f(Shader *self, const char *name, float x, float y, float z,
        float w);
void shader_set_vec3f(Shader *self, const char *name, float x, float y, float
        z);
void shader_set_mat4(Shader *self, const char *name, float value[4][4]);


# endif
