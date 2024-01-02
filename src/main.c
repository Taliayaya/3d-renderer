# include "../include/glad/glad.h"
# include <math.h>
# include <GLFW/glfw3.h>
# include <stdlib.h>
# include <err.h>
# include <stdbool.h>

# include "../include/shader.h"
# define STB_IMAGE_IMPLEMENTATION
# include "../include/stb_image.h"
# include "../include/math/utils.h"
# include "../include/math/matrix.h"
# include "../include/math/vec4.h"
# include "../include/math/vec3.h"
# include "../include/perspective.h"
# include "../include/transform.h"
# include "../include/camera.h"


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

# define CLAMP(value, low, high)

Camera *camera;
Vec3 camera_front = {0.f, 0.f, -1.f};

float yaw = -90.f;
float pitch = 0;

float last_x = WIDTH / 2;
float last_y = HEIGHT / 2;

float delta_time = 0.f;
float last_frame = 0.f;

float fov = 45.f;

// called each time the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static bool first_mouse = true;
    // avoid sudden jump when focusing
    if (first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = ypos - last_y;
    last_x = xpos;
    last_y = ypos;
    
    camera_process_mouse(camera, xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera_process_scroll(camera, yoffset);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    enum CameraMovement direction;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        direction = FORWARD;
        camera_process_keyboard(camera, direction, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        direction = BACKWARD;
        camera_process_keyboard(camera, direction, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        direction = LEFT;
        camera_process_keyboard(camera, direction, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        direction = RIGHT;
        camera_process_keyboard(camera, direction, delta_time);
    }
}

int main()
{

    Vec3 camera_pos = {0, 0, 3.f};
    Vec3 up = {0, 1.f, 0};
    camera = new_camera(&camera_pos, &up);


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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // init glad to call any OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        err(EXIT_FAILURE, "Failed to initialize GLAD");


    glEnable(GL_DEPTH_TEST);
    //glViewport(0, 0, WIDTH, HEIGHT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_FILL

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
    Vec3 cube_positions[] = {
        { 0.0f,  0.0f,  0.0f},
        { 2.0f,  5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        { 2.4f, -0.4f, -3.5f},
        {-1.7f,  3.0f, -7.5f},
        { 1.3f, -2.0f, -2.5f},
        { 1.5f,  2.0f, -2.5f},
        { 1.5f,  0.2f, -1.5f},
        {-1.3f,  1.0f, -1.5f}
    };

    unsigned int texture0, texture1;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
    // register mouse event
    glfwSetCursorPosCallback(window, mouse_callback);
    // register scroll event
    glfwSetScrollCallback(window, scroll_callback);

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

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
    //        GL_STATIC_DRAW);

    // tell openGL how to interpret the vertex data in memory
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
            (void *)0);

    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
    //        (void *)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
            (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // unbind the VA0 so that others dont modify it
    glBindVertexArray(0);

    Vec3 rotate_axis = {1.f, .3f, .5f};

    while (!glfwWindowShouldClose(window))
    {
        // begin frame
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // clear depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // inputs
        processInput(window);

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();

        Mat4 *view = camera_look_at(camera);

        Mat4 *projection = perspective(TO_RAD(camera->zoom), (float)WIDTH / HEIGHT, .1f, 100.f);


        shader_use(shader);
        shader_set_int(shader, "texture1", 1);
        shader_set_mat4(shader, "view", view->arr);
        shader_set_mat4(shader, "projection", projection->arr);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // copy vertices array to buffer for OGL
        glBindVertexArray(VAO);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        for (int i = 0; i < 10; ++i)
        {
            Mat4 *model = new_mat4_id(1.f);
            model = mat4_translate(model, &cube_positions[i]);
            float angle = 20.f * i * timeValue;
            model = mat4_rotate(model, TO_RAD(angle), &rotate_axis);
            shader_set_mat4(shader, "model", model->arr);
            free_mat4(model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(9);

        free_mat4(view);
        free_mat4(projection);


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
