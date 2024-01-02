# ifndef CAMERA_H
# define CAMERA_H

# include <err.h>
# include <stdbool.h>

# include "../include/math/vec3.h"
# include "../include/math/matrix.h"
# include "../include/math/utils.h"
# include "../include/transform.h"

# define MAX_FOV 45.f
# define MIN_FOV 1.f

# define YAW              -90.f;
# define PITCH             0.0f;
# define SPEED             2.5f;
# define SENSITIVITY       0.1f;
# define ZOOM              45.f;
# define ZOOM_SENSITIVITY  2.5f;

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

typedef struct Camera
{
    Vec3 world_up;
    Transform transform;

    float yaw;
    float pitch;

    float movement_speed;
    float mouse_sensitivity;
    float zoom;
    float zoom_sensitivity;
} Camera;

Camera *new_camera(Vec3 *position, Vec3 *up);

void free_camera(Camera *camera);

void camera_update_vectors(Camera *camera);

Mat4 *camera_look_at(Camera *camera);

void camera_process_keyboard(Camera *camera, enum CameraMovement direction, float
        delta_time);

void camera_process_mouse(Camera *camera, float xoffset, float yoffset,
        bool constrain_pitch);

void camera_process_scroll(Camera *camera, float yoffset);

# endif
