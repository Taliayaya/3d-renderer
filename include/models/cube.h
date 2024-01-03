# ifndef CUBE_H
# define CUBE_H

# include "../../include/glad/glad.h"

# include <stdbool.h>


# define CUBE_SIDE1 0
# define CUBE_SIDE2 1
# define CUBE_SIDE3 2
# define CUBE_SIDE4 3
# define CUBE_BOT   4
# define CUBE_TOP   5

typedef struct Cube
{

    unsigned int VAO;
    unsigned int VBO;

    unsigned int textures[6];
} Cube;

Cube new_cube(void);
void cube_draw(Cube *cube);

# endif
