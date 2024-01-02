# include "../include/camera.h"

Camera *new_camera(Vec3 *position, Vec3 *up)
{
    Camera *camera = calloc(1, sizeof(Camera));
    if (camera == NULL)
        err(EXIT_FAILURE, "new_camera: Couldn't allocate the camera");

    camera->world_up = *up;
    camera->transform.position = *position;
    camera->zoom = ZOOM;
    camera->pitch = PITCH;
    camera->mouse_sensitivity = SENSITIVITY;
    camera->movement_speed = SPEED;
    camera->zoom_sensitivity = ZOOM_SENSITIVITY;
    
    camera_update_vectors(camera);

    return camera;
}

void camera_update_vectors(Camera *camera)
{
    Vec3 direction;
    direction.x = cos(TO_RAD(camera->yaw)) * cos(TO_RAD(camera->pitch));
    direction.y = sin(TO_RAD(camera->pitch));
    direction.z = sin(TO_RAD(camera->yaw)) * cos(TO_RAD(camera->pitch));
    Vec3 camera_front = vec3_normalized(&direction);


    Vec3 right = vec3_cross(&camera->world_up, &direction);
    vec3_normalize(&right);

    Vec3 cam_up = vec3_cross(&direction, &right);
    vec3_normalize(&cam_up);

    camera->transform.forward = camera_front;
    camera->transform.right = right;
    camera->transform.up = cam_up;
}

void free_camera(Camera *camera)
{
    free(camera);
}

Mat4 *camera_look_at(Camera *camera)
{
    Vec3 x_axis, y_axis, z_axis;

    z_axis = camera->transform.forward;
    vec3_normalize(&z_axis);

    x_axis = camera->transform.right;
    vec3_normalize(&x_axis);

    y_axis = camera->transform.up;
    vec3_negate(&z_axis);

    Mat4 *view = new_mat4_id(1.f);
    // first row
    view->arr[0][0] = x_axis.x;
    view->arr[0][1] = x_axis.y;
    view->arr[0][2] = x_axis.z;
    view->arr[0][3] = -vec3_dot(&x_axis, &camera->transform.position);

    // second row
    view->arr[1][0] = y_axis.x;
    view->arr[1][1] = y_axis.y;
    view->arr[1][2] = y_axis.z;
    view->arr[1][3] = -vec3_dot(&y_axis, &camera->transform.position);

    // third row
    view->arr[2][0] = z_axis.x;
    view->arr[2][1] = z_axis.y;
    view->arr[2][2] = z_axis.z;
    view->arr[2][3] = -vec3_dot(&z_axis, &camera->transform.position);

    return view;
}

void camera_process_keyboard(Camera *camera, enum CameraMovement direction, float
        delta_time)
{
    const float velocity = camera->movement_speed * delta_time;
    if (direction == FORWARD)
    {
        Vec3 forw = vec3_mul(&camera->transform.forward, velocity);
        camera->transform.position = vec3_add(&camera->transform.position, &forw);
    }
    if (direction == BACKWARD)
    {
        Vec3 forw = vec3_mul(&camera->transform.forward, velocity);
        camera->transform.position = vec3_sub(&camera->transform.position, &forw);
    }
    if (direction == LEFT)
    {
        Vec3 right = vec3_mul(&camera->transform.right, velocity);
        camera->transform.position = vec3_sub(&camera->transform.position,
                &right);
    }
    if (direction == RIGHT)
    {
        Vec3 right = vec3_mul(&camera->transform.right, velocity);
        camera->transform.position = vec3_add(&camera->transform.position,
                &right);
    }
}

void camera_process_mouse(Camera *camera, float xoffset, float yoffset,
        bool constrain_pitch)
{
    xoffset *= camera->mouse_sensitivity;
    yoffset *= camera->mouse_sensitivity;
    camera->yaw   -= xoffset;
    camera->pitch -= yoffset;

    // avoid loss of precision of float
    camera->yaw = fmod(camera->yaw, 360.f);

    if (constrain_pitch)
        camera->pitch = clampf(camera->pitch, -89.f, 89.f);
    camera_update_vectors(camera);
}

void camera_process_scroll(Camera *camera, float yoffset)
{
    camera->zoom -= (float)yoffset * camera->zoom_sensitivity;
    camera->zoom = clampf(camera->zoom, MIN_FOV, MAX_FOV);
}
