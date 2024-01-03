# ifndef TEXTURE_H
# define TEXTURE_H

# include "../include/glad/glad.h"
# include <stdlib.h>
# include <err.h>
# include <stdbool.h>


typedef struct Texture
{
    unsigned int ID;

    // texture info
    int width;
    int height;
    int nr_channels;

} Texture;

Texture new_texture(const char *path);

# endif
