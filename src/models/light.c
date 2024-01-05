# include "../../include/models/light.h"

Cube new_light(void)
{
    Cube light = new_cube();
    
    static bool vertex_init = true;
    static unsigned int VAO;
    if (vertex_init)
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        // we reuse the same VBO
        glBindBuffer(GL_ARRAY_BUFFER, light.VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void
                    *)0);
        glEnableVertexAttribArray(0);

    }
    light.VAO = VAO;
    return light;
}

void light_draw(Cube *cube)
{
    glBindVertexArray(cube->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
