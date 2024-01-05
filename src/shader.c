# include "../include/shader.h"

/**
  * Read the whole stream and heap allocate a large enough memory space
  * into a string.
  */
int read_whole_file(int fd, const char **output_str)
{
    int max_len = 64;
    int len = 0;
    char *str = malloc((max_len + 1) * sizeof(char));

    int r;
    while ((r = read(fd, str + len, max_len - len)) != 0)
    {
        if (r == -1)
            err(EXIT_FAILURE, "read file");
        len += r;
        if (len == max_len)
        {
            max_len *= 2;
            str = realloc(str, (max_len + 1) * sizeof(char));
        }
    }
    str[len] = 0;
    *output_str = (const char *)str;
    return len;
}

/**
  * Compile the given shader code and return its id
  */
unsigned int compile_shader(const char *code, int flag)
{
    unsigned int shader_id;
    int success;
    char infoLog[512];

    // vertex shader
    shader_id = glCreateShader(flag);
    glShaderSource(shader_id, 1, &code, NULL);
    glCompileShader(shader_id);

    // print compile error if exists
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
        errx(EXIT_FAILURE, "compile_shader: %.*s\n", 512, infoLog);
    }
    return shader_id;
}

/**
  * Link the two shaders and delete the shader once done
  */
void shader_program(Shader *shader, unsigned int vertex, unsigned int fragment)
{
    shader->ID = glCreateProgram();
    glAttachShader(shader->ID, vertex);
    glAttachShader(shader->ID, fragment);
    glLinkProgram(shader->ID);
    // print linking error if there are any
    int success;
    glGetProgramiv(shader->ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shader->ID, 512, NULL, infoLog);
        errx(EXIT_FAILURE, "program linking failed\n%.*s\n", 512, infoLog);
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


/**
  * Read the two shader codes, compile them, link them and return
  * the ID of the program
  */
Shader *new_shader(const char *vertexPath, const char *fragmentPath)
{
    Shader *shader = malloc(sizeof(Shader));
    const char *vertexCode, *fragmentCode;

    {
        int vertex_fd = open(vertexPath, O_RDONLY);
        if (vertex_fd == -1)
            err(EXIT_FAILURE, "open vertex");

        read_whole_file(vertex_fd, &vertexCode);
        printf("%s\n", vertexCode);
        close(vertex_fd);
    }

    {
        int frag_fd = open(fragmentPath, O_RDONLY);
        if (frag_fd == -1)
            err(EXIT_FAILURE, "open fragment");

        read_whole_file(frag_fd, &fragmentCode);
        printf("%s\n", fragmentCode);
        close(frag_fd);
    }
    unsigned int vertex, fragment;
    vertex = compile_shader(vertexCode, GL_VERTEX_SHADER);
    fragment = compile_shader(fragmentCode, GL_FRAGMENT_SHADER);
    shader_program(shader, vertex, fragment);
    free((char *)vertexCode);
    free((char *)fragmentCode);
    return shader;
}

void shader_use(Shader *self)
{
    glUseProgram(self->ID);
}

void shader_set_bool(Shader *self, const char *name, bool value)
{
    glUniform1i(glGetUniformLocation(self->ID, name), (int)value);
}

void shader_set_int(Shader *self, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(self->ID, name), value);
}

void shader_set_float(Shader *self, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(self->ID, name), value);
}

void shader_set_mat4(Shader *self, const char *name, float value[4][4])
{
    int location = glGetUniformLocation(self->ID, name);
    glUniformMatrix4fv(location, 1, GL_TRUE, &value[0][0]);
}


void shader_set_vec4f(Shader *self, const char *name, float x, float y, float z,
        float w)
{
    int location = glGetUniformLocation(self->ID, name);
    glUniform4f(location, x, y, z, w);
}

void shader_set_vec3f(Shader *self, const char *name, float x, float y, float
        z)
{
    int location = glGetUniformLocation(self->ID, name);
    glUniform3f(location, x, y, z);
}

void free_shader(Shader *self)
{
    if (self == NULL)
        return;
    glDeleteProgram(self->ID);
    free(self);
}
