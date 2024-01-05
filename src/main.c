# include "../include/glad/glad.h"
# include <math.h>
# include <GLFW/glfw3.h>
# include <stdlib.h>
# include <err.h>
# include <stdbool.h>
# include <time.h>

# include "../include/shader.h"
# include "../include/math/utils.h"
# include "../include/math/matrix.h"
# include "../include/math/vec4.h"
# include "../include/math/vec3.h"

# include "../include/perspective.h"
# include "../include/transform.h"
# include "../include/camera.h"
# include "../include/texture.h"

// models
# include "../include/models/cube.h"
# include "../include/models/light.h"

// map
# include "../include/perlin_noise.h"


// window settings
# define WIDTH 800
# define HEIGHT 600
# define TITLE "Minecraft"

// opengl settings
# define MAJOR 3
# define MINOR 3

// shaders path
# define VERTEX_SHADER_PATH "../shaders/shader.vs"
# define FRAGMENT_SHADER_PATH "../shaders/shader.fs"

# define LIGHT_VERTEX_SHADER_PATH "../shaders/light.vs"
# define LIGHT_FRAGMENT_SHADER_PATH "../shaders/light.fs"

// texture path
# define TEXTURE_PATH "../textures/"

# define CLAMP(value, low, high)

# define MAP_WIDTH 50
# define MAP_HEIGHT 50

extern int SEED;

Camera *camera;
Vec3 camera_front = {0.f, 0.f, -1.f};

float yaw = -90.f;
float pitch = 0;

float last_x = WIDTH / 2;
float last_y = HEIGHT / 2;

float delta_time = 0.f;
float last_frame = 0.f;

float fov = 45.f;

Vec3 light_pos = {30, 20.f, 30.f};

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
    SEED = time(NULL);

    Vec3 camera_pos = {0, 0, 3.f};
    Vec3 up = {0, 1.f, 0};
    camera = new_camera(&camera_pos, &up);

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

    Vec3 map[MAP_WIDTH * MAP_HEIGHT];
    for (int i = 0; i < MAP_WIDTH; ++i)
        for (int j = 0; j < MAP_HEIGHT; ++j)
        {
            float y = perlin2d(i, j, 0.1, 20);
            y = round(lerpf(y, 1, 20));
            map[i * MAP_HEIGHT + j] = (Vec3){i, y, j};
        }

    Texture grass_bot, grass_side, grass_top;

    grass_bot = new_texture(TEXTURE_PATH"grass_bot.jpg"); 
    grass_side = new_texture(TEXTURE_PATH"grass_side.jpg");
    grass_top = new_texture(TEXTURE_PATH"grass_top.jpg");

    // create, attach shaders to the program and link them
    Shader *shader = new_shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    Shader *light = new_shader(LIGHT_VERTEX_SHADER_PATH,
                               LIGHT_FRAGMENT_SHADER_PATH);

        // register the event for resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // register mouse event
    glfwSetCursorPosCallback(window, mouse_callback);
    // register scroll event
    glfwSetScrollCallback(window, scroll_callback);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    Cube grass = new_cube();
    for (int i = CUBE_SIDE1; i <= CUBE_SIDE4; ++i)
        grass.textures[i] = grass_side.ID;
    grass.textures[CUBE_BOT] = grass_bot.ID;
    grass.textures[CUBE_TOP] = grass_top.ID;

    Cube dirt = new_cube();
    for (int i = CUBE_SIDE1; i <= CUBE_TOP; ++i)
        dirt.textures[i] = grass_bot.ID;

    Cube lightCube = new_light();

    
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

        float timeValue = glfwGetTime();
        float light_x = light_pos.x + 50 * sin(timeValue / 4);
        float light_y = light_pos.y + 50 * cos(timeValue / 4);

        float bg = light_y / (light_pos.y + 50);
        if (bg < 0.1)
            bg = 0.1;
        glClearColor(bg *.5, bg * .8, bg * 1.2, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        Mat4 view = camera_look_at(camera);

        Mat4 projection = perspective(TO_RAD(camera->zoom), (float)WIDTH / HEIGHT, .1f, 100.f);

        Vec3 light_diffuse = {.5f, .5f, .5f};
        float maxLight = light_y / (light_pos.y + 30);
        //light_diffuse.x = lerpf(clampf(light_y / maxLight, 0, 1), 0,
        //        light_diffuse.x); 
        //light_diffuse.y = lerpf(clampf(light_y / maxLight, 0, 1), 0,
        //        light_diffuse.y);
        //light_diffuse.z = lerpf(clampf(light_y / maxLight, 0, 1), 0,
        //        light_diffuse.z);

        Vec3 light_specular = {1.f, 1.f, 1.f};
//        light_specular.x = lerpf(clampf(light_y / maxLight, 0, 1), 0,
//                light_specular.x); 
//        light_specular.y = lerpf(clampf(light_y / maxLight, 0, 1), 0,
//                light_specular.y);
//        light_specular.z = lerpf(clampf(light_y / maxLight, 0, 1), 0,
//                light_specular.z);
//
        Vec3 light_color = {1.f, 1.f, 1.f};
        
        if (light_y < 30)
        {
            Vec3 orange = (Vec3){1.f, .2f, 0.f};
            light_color.x = lerpf(light_y / 30, orange.x, light_color.x);
            light_color.y = lerpf(light_y / 30, orange.y, light_color.y);
            light_color.z = lerpf(light_y / 30, orange.z, light_color.z);
        }
        if (light_y < 0)
            light_color = (Vec3){0.1f, 0.1f, 0.1f};


        shader_use(shader);
        shader_set_mat4(shader, "view", view.arr);
        shader_set_mat4(shader, "projection", projection.arr);
        shader_set_vec3f(shader, "material.ambient", .11f, .11f, .11f);
        shader_set_vec3f(shader, "material.diffuse", .3f, .3f, .3f);
        shader_set_vec3f(shader, "material.specular", .5f, .5f, .5f);
        shader_set_float(shader, "material.shininess", 32.f);
        shader_set_vec3f(shader, "lightColor", light_color.x, light_color.y,
                light_color.z);
        shader_set_vec3f(shader, "lightPos", light_x, light_y, light_pos.z);
        shader_set_vec3f(shader, "viewPos", camera->transform.position.x,
                                            camera->transform.position.y,
                                            camera->transform.position.z);
        shader_set_vec3f(shader, "light.ambient", .2f, .2f, .2f);
        shader_set_vec3f(shader, "light.diffuse", light_diffuse.x,
                light_diffuse.y, light_diffuse.z);
        shader_set_vec3f(shader, "light.specular", light_specular.x,
                light_specular.y, light_specular.z);
        glActiveTexture(GL_TEXTURE0);
                
        // copy vertices array to buffer for OGL
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // light
        Mat4 lmodel = new_mat4_id(1.f);
        Vec3 light_postmp = {light_x, light_y, light_pos.x};
        lmodel = mat4_translate(&lmodel, &light_postmp);
        Vec3 lscale = {1.5, 1.5, 1.5};
        lmodel = mat4_scale(&lmodel, &lscale);

        shader_use(light);
        shader_set_mat4(light, "view", view.arr);
        shader_set_mat4(light, "projection", projection.arr);
        shader_set_mat4(light, "model", lmodel.arr);

        light_draw(&lightCube);

        shader_use(shader);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i)
        {
            Mat4 model = new_mat4_id(1.f);
            model = mat4_translate(&model, &map[i]);
            shader_set_mat4(shader, "model", model.arr);

            cube_draw(&grass);           

            Vec3 pos = map[i];
            pos.y = map[i].y - 1;
            
            while (pos.y >= 0)
            {
                model = new_mat4_id(1.f);
                model = mat4_translate(&model, &pos);
                shader_set_mat4(shader, "model", model.arr);

                cube_draw(&dirt);
                pos.y--;
            }
            
        }
        glBindVertexArray(0);

        // call events and swap buffers
        glfwSwapBuffers(window); // avoid flickering
        glfwPollEvents(); // are events triggered?
    }
    glDeleteVertexArrays(1, &grass.VAO);
    glDeleteBuffers(1, &grass.VBO);
    free_shader(shader);

    // clean
    glfwTerminate();
    return 0;
}
