
#include "camera.h"

Camera::Camera()
{
    _field_of_view = 45.0f;

    _dist_for_near_plane = -10.0f;
    _dist_for_far_plane = 10;

    // _dist_for_near_plane = 10.0f;
    // _dist_for_far_plane = 1000.f;

    _width = 800.0f;
    _height = 600.0f;

    P = glm::mat4(1.f);
}

void Camera::build_proj_ortho()
{
    float x1 = -_width*0.5f;
    float x2 = _width*0.5f;
    float y1 = -_height*0.5f;
    float y2 = _height*0.5f;

    P = glm::ortho (x1, x2, y1, y2, _dist_for_near_plane, _dist_for_far_plane);
}

void Camera::build_proj_perspective()
{
    float aspect_ratio = _width/_height;

    P = glm::perspective(glm::radians(_field_of_view), aspect_ratio, _dist_for_near_plane, _dist_for_far_plane);
}
