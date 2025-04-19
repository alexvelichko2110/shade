#ifndef CAMERA_H_
#define CAMERA_H_

#include "glm_includes.h"

#include "node.h"

class Camera
{
public:

    Camera();

    void set_width(float width) {
        _width = width;
    }

    void set_height(float height) {
        _height = height;
    }

    void set_distance_for_near_plane(float near) {
        _dist_for_near_plane = near;
    }

    void set_distance_for_far_plane(float far) {
        _dist_for_far_plane = far;
    }

    void set_field_of_view(float fov) {
        _field_of_view = fov;
    }

    glm::mat4 get_proj_matrix() {
        return P;
    }

    void build_proj_ortho();
    void build_proj_perspective();

    // attach camera this to node 
    Node* get_node() {
        return _node;
    }

    void set_node(Node* node) {
        _node = node;
    }

private:

    Node* _node;

    // projection matric
    glm::mat4 P;

    float _width;
    float _height;

    float _dist_for_near_plane;
    float _dist_for_far_plane;

    float _field_of_view;
};


#endif //CAMERA_H_
