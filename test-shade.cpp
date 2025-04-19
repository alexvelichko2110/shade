
#include <string>
#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <vector>

// #include <opencv2/opencv.hpp>
// #include <opencv2/imgproc/imgproc_c.h>


#include "shader.h"
#include "buffer.h"
#include "texture.h"
#include "font.h"
#include "node.h"
#include "camera.h"
#include "render.h"
// #include "font_render.h"
// #include "line_render.h"
// #include "rect_render.h"

#include "resource_manager.h"

// #include "flat_objects.h"

#include "framebuffer.h"

// Window dimensions
GLuint WIDTH = 1024, HEIGHT = 768;


bool keys[1024];

bool mouse_press = false;

float lastX = 400;
float lastY = 300;

bool firstMouse = true;

Node node;
Node node_cam;

Node root;

float PITCH = WIDTH*0.5;
float YAW = HEIGHT* 0.5;

float cam_alpha = 270.0;
float cam_radius = 300.0;


void error ( int error, const char * description )
{
    fputs ( description, stderr );
}


void key_callback ( GLFWwindow * window, int key, int scancode, int action, int mods )
{
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        glfwSetWindowShouldClose ( window, GL_TRUE );

    if ( key == GLFW_KEY_W && action == GLFW_PRESS)
        cam_alpha -= 10.0;

    if ( key == GLFW_KEY_E && action == GLFW_PRESS)
        cam_alpha += 10.0;

    if ( key == GLFW_KEY_UP && action == GLFW_PRESS)
        cam_radius -= 10.0;

    if ( key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        cam_radius += 10.0;

   
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to leTCHft

    lastX = xpos;
    lastY = ypos;

    // std::cout << "mouse xoffset " << xoffset << " yoffset " << yoffset << std::endl;

    if (mouse_press) {

        PITCH += xoffset;
        YAW += yoffset;     

        node.set_pitch (PITCH);
        node.set_yaw (YAW);
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        mouse_press = true;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        mouse_press = false;
    }
}



void load_resources_all()
{
    rm()->load_shader("rect_solid", "./resources/simple_one_mat.vs", "./resources/rect_solid.fs");
    rm()->load_shader("rect_tex", "./resources/simple_one_mat.vs", "./resources/rect_texture.fs");

    rm()->load_shader("text", "./resources/text.vs", "./resources/text.fs");
    rm()->load_shader("line", "./resources/line.vs", "./resources/line.fs");

    rm()->load_shader("poly", "./resources/simple_one_mat.vs", "./resources/poly.fs");
    rm()->load_shader("line1", "./resources/simple_one_mat.vs", "./resources/lines.fs");
    
    rm()->load_texture("tex1", "./resources/tex1.jpeg");

    rm()->load_font("font1", "./resources/RDR.ttf");
}


int main( int argc, char** argv ) 
{
    // Init GLFW
    glfwInit();

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
   
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "pano demo", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);


    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;

    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();


    Camera camera;

    camera.set_width(WIDTH);
    camera.set_height(HEIGHT);

    camera.set_distance_for_near_plane(0.1);
    camera.set_distance_for_far_plane(10000);

    camera.build_proj_perspective();


    // node for pano
    node.set_yaw(90.0);
    node.set_pitch(0.0);

    node.set_position(glm::vec3(0.0, 0.0, 0.0));

    // attach camera to its node
    camera.set_node(&node_cam);


    load_resources_all();

    // FrameBuffer framebuffer(600, 400);

    Render render;

    render.set_framebuffer_size(WIDTH, HEIGHT);
    render.init();

    Buffer* buffer_plane = new Buffer(GL_LINES);
    buffer_plane->make_buffers();

    while ( glfwWindowShouldClose ( window ) != true)
    {

        // Check if any events have been activiated (key pressed, mouse moved etc.) 
        // and call corresponding response functions
        glfwPollEvents();


        buffer_plane->reset();

        for (int ii = 0; ii <= 100; ii++) {

            float hh = 1000/100;

            buffer_plane->add_point(glm::vec3(-500.0, 0.0, -500 + ii*hh));
            buffer_plane->add_point(glm::vec3(500.0, 0.0, -500 + ii*hh));

        }

        
        for (int ii = 0; ii <= 100; ii++) {

            float hh = 1000/100;

            buffer_plane->add_point(glm::vec3(-500.0 + ii*hh, 0.0, -500.0));
            buffer_plane->add_point(glm::vec3(-500.0 + ii*hh, 0.0, 500.0));
        }


        buffer_plane->make_line();
        buffer_plane->load_to_gpu();


        // setup camera

        glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);


        float camx = cam_radius*sin(glm::radians(cam_alpha));
        float camz = cam_radius*cos(glm::radians(cam_alpha));


        // glm::vec3 pos = glm::vec3(150.0, 40.0, 230.0);

        glm::vec3 pos = glm::vec3(camx, 40.0, camz);
        glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);

        node_cam.look_at(pos, target, up);
        node_cam.build_this();


        node.euler();
        node.build_this();


        root.build_this();


        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // draw all objects

        render.set_viewport(0, 0, WIDTH, HEIGHT);
        render.clear();

        render.depth(true);

        glLineWidth(1.0);

        // draw line
        Shader* shader2 = rm()->shaders("line1").get();

        render.draw_buffer_u(buffer_plane, &root, &camera, shader2, false);


        render.depth(false);

        // glLineWidth(2.0);


        render.reset();

        Font* font = rm()->fonts("font1").get();

        glm::mat4 M = glm::mat4(1.0f);
        M = glm::translate(M, glm::vec3(100, 100, 0.0f));

        render.set_transform(M);
        render.set_color(glm::vec4(1.0, 1.0, 1.0, 1.0));
        render.set_font(font);
        render.draw_text("hello", 40.0f, 100.0f);


       // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
