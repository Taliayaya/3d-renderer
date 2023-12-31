# include "../include/glad/glad.h"
# include <math.h>
# include <GLFW/glfw3.h>
# include <stdlib.h>
# include <err.h>
# include <stdbool.h>

# include "../include/shader.h"
# define STB_IMAGE_IMPLEMENTATION
# include "../include/stb_image.h"




// window settings
# define WIDTH 800
# define HEIGHT 600
# define TITLE "LearnOpenGL"

// opengl settings
# define MAJOR 3
# define MINOR 3

// shaders path
# define VERTEX_SHADER_PATH "../shaders/shader.vs"
# define FRAGMENT_SHADER_PATH "../shaders/shader.fs"

// texture path
# define TEXTURE_PATH "../textures/"


// called each time the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    

    stbi_set_flip_vertically_on_load(true);
    glfwInit();
    // tells opengl which version we want to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR);
    // only use core
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        err(EXIT_FAILURE, "CreateWindow");
    }
    // render window
    glfwMakeContextCurrent(window);

    // init glad to call any OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        err(EXIT_FAILURE, "Failed to initialize GLAD");

    //glViewport(0, 0, WIDTH, HEIGHT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_FILL


    // my triangle (no z coord)
    float vertices[] = 
    {
        // positions          // colors          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // top right
         0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,  0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // top left
         0.0f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,  0.5f, 1.0f   // top

    };

    unsigned int indices[] = 
    {
        0, 1, 3,
        1, 2, 3
    };

    float texCoords[] = 
    {
        0.0f, 0.0f, // low left
        1.0f, 0.0f, // low right
        0.5f, 1.0f  // top center
    };
    unsigned int texture0, texture1;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(TEXTURE_PATH"container.jpg", &width,
            &height, &nrChannels, 0);
    if (!data)
        err(EXIT_FAILURE, "failed to load texture");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    data = stbi_load(TEXTURE_PATH"awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // create, attach shaders to the program and link them
    Shader *shader = new_shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    


    // register the event for resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    unsigned int VBO; // vertex buffer objects
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);



    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    
    // copies the previously defined vertex into the buffer's memory
    // size of data in BYTES
    // static_draw => data set once, used plenty (our triangle does not change)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
            GL_STATIC_DRAW);

    // tell openGL how to interpret the vertex data in memory
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void *)0);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // unbind the VA0 so that others dont modify it
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        // begin frame

        // inputs
        processInput(window);

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

        shader_use(shader);
        shader_set_vec4f(shader, "ourColor", 0.0f, greenValue, 0.0f, 1.0f);
        shader_set_float(shader, "offset", 0.f);
        shader_set_int(shader, "texture1", 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // copy vertices array to buffer for OGL
        glBindVertexArray(VAO);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(9);


        // call events and swap buffers
        glfwSwapBuffers(window); // avoid flickering
        glfwPollEvents(); // are events triggered?
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    free_shader(shader);

    // clean
    glfwTerminate();
    return 0;
}
