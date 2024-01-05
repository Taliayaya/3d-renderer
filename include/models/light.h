# ifndef LIGHT_H
# define LIGHT_H

# include "../../include/glad/glad.h"
# include <stdbool.h>

# include "cube.h"

Cube new_light(void);
void light_draw(Cube *light);


# endif
