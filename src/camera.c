# include "../include/camera.h"

Camera *new_camera(Vec3 *position, Vec3 *target, Vec3 *up)
{
    Camera *camera = calloc(1, sizeof(Camera));
    if (camera == NULL)
        err(EXIT_FAILURE, "new_camera: Couldn't allocate the camera");

    Vec3 direction = vec3_sub(position, target);
    vec3_normalize(&direction);

    Vec3 right = vec3_cross(up, &direction);
    vec3_normalize(&right);

    Vec3 cam_up = vec3_cross(&direction, &right);
    vec3_normalize(&cam_up);

    camera->target = *target;
    camera->transform.position = *position;
    camera->transform.forward = direction;
    camera->transform.right = right;
    camera->transform.up = cam_up;
    
    return camera;
}

void free_camera(Camera *camera)
{
    free(camera);
}

Mat4 *camera_look_at(Camera *camera)
{
    Vec3 x_axis, y_axis, z_axis;

    z_axis = vec3_sub(&camera->target, &camera->transform.position);
    vec3_normalize(&z_axis);

    x_axis = vec3_cross(&z_axis, &camera->transform.up);
    vec3_normalize(&x_axis);

    y_axis = vec3_cross(&x_axis, &z_axis);

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
