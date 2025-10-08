
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
#include "mesh.h"
#include "texture.h"
#include "font.h"
#include "node.h"
#include "camera.h"
#include "render.h"

#include "resource_manager.h"

#include "framebuffer.h"

// Window dimensions
GLuint WIDTH = 1400, HEIGHT = 900;


bool keys[1024];

bool mouse_press = false;

float lastX = 400;
float lastY = 300;

bool firstMouse = true;

Node node;
Node node_cam;

// Node root;

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

        PITCH += xoffset*0.3;
        YAW += yoffset*0.3;     

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


glm::mat4 get_mat_node(float x, float y, float z, float angle, float angle1)
{
    glm::mat4 M = glm::mat4(1.0f);
    M = glm::translate(M, glm::vec3(x, y, z));
    M = glm::rotate(M, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    M = glm::rotate(M, angle1, glm::vec3(0.0f, 0.0f, 1.0f));

    return M;
}



glm::mat4 get_mat_node(float x, float y, float angle)
{
    glm::mat4 M = glm::mat4(1.0f);
    M = glm::translate(M, glm::vec3(x, y, 0.0f));
    M = glm::rotate(M, angle, glm::vec3(0.0f, 0.0f, 1.0f));

    return M;
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "cube demo ...", nullptr, nullptr);
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
    node.set_yaw(60.0);
    node.set_pitch(10.0);

    node.set_position(glm::vec3(0.0, 0.0, 0.0));
    
    // attach camera to its node
    camera.set_node(&node_cam);


    // FrameBuffer framebuffer(600, 400);

    Render render;
    render.load_resources_all();

    rm()->load_font("font1", "./resources/RDR.ttf");


    render.set_framebuffer_size(WIDTH, HEIGHT);
    render.init();


    Texture *cube_tex = Texture::load("./resources/tex2.jpeg");

    Mesh cube;

    cube.load_OBJ("../../untitled.obj");

    ///////
    
    GLuint VAO, VBO;//, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cube._data_stride*cube._vertex_count, cube._vertexlist, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);


    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // // TexCoord attribute
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    // glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO
    ////

    // Node node1;

    // node.set_yaw(30.0);
    // node.set_pitch(60.0);

    // node.set_position(glm::vec3(10.0, 10.0, 10.0));

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    char buff[256];

    
    while ( glfwWindowShouldClose ( window ) != true)
    {
        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
            // printf and reset timer
            // printf("%f ms/frame\n", 1000.0/double(nbFrames));
            sprintf(buff, "frame %d", nbFrames);

            nbFrames = 0;
            lastTime += 1.0;
        }

        // Check if any events have been activiated (key pressed, mouse moved etc.) 
        // and call corresponding response functions
        glfwPollEvents();


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
        
 
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // draw all objects

        render.set_viewport(0, 0, WIDTH, HEIGHT);
        render.clear();

        render.depth(true);

 
        glm::mat4 P = camera.get_proj_matrix();
        glm::mat4 V = camera.get_node()->get_mat();
        // glm::mat4 M = root.get_mat_inverse();//glm::mat4(1.0f);
        glm::mat4 M = glm::mat4(1.0f);

        render.set_proj_mat(P);
        render.set_view_mat(V);
        
        // draw plane
        render.set_transform(M);
        render.set_color(glm::vec4(0.3, 0.3, 0.3, 1.0));
        render.set_line_width(1.0f);

        for (int ii = 0; ii <= 100; ii++) {

            float hh = 1000/100;

            render.draw_line(glm::vec3(-500.0, 0.0, -500 + ii*hh), glm::vec3(500.0, 0.0, -500 + ii*hh));
            render.draw_line(glm::vec3(-500.0 + ii*hh, 0.0, -500.0), glm::vec3(-500.0 + ii*hh, 0.0, 500.0));
     
        }
        // render.depth(false);

        // draw axis
        M = get_mat_node(0,0,0,PITCH, YAW);//node.get_mat_inverse();
        render.set_transform(M);
        
        render.set_color(glm::vec4(1.0, 0.0, 0.0, 1.0));
        render.draw_line(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 100.0, 0.0));

        render.set_color(glm::vec4(0.0, 1.0, 0.0, 1.0));
        render.draw_line(glm::vec3(0.0, 0.0, 0.0), glm::vec3(100.0, 0.0, 0.0));

        render.set_color(glm::vec4(0.0, 0.0, 1.0, 1.0));
        render.draw_line(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 100.0));


        // draw cube

        // glm::mat4 M1 = node.get_mat_inverse();
        
        glm::mat4 M1 = M*get_mat_node(50, 20, 50, 40.0, 60.0);
        
        render.set_transform(M1);
        
        render.set_color(glm::vec4(1.0, 0.0, 0.0, 1.0));
        render.draw_line(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 100.0, 0.0));

        render.set_color(glm::vec4(0.0, 1.0, 0.0, 1.0));
        render.draw_line(glm::vec3(0.0, 0.0, 0.0), glm::vec3(100.0, 0.0, 0.0));

        render.set_color(glm::vec4(0.0, 0.0, 1.0, 1.0));
        render.draw_line(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 100.0));


        Shader* shader = rm()->shaders("rect_tex").get();
        		
        shader->use();

        glActiveTexture ( GL_TEXTURE0 );    
        glBindTexture ( GL_TEXTURE_2D, cube_tex->get_texID() );

        shader->set("ourTexture", 0);


        M1 = glm::scale(M1, glm::vec3(20.0));

        shader->set("M", P*V*M1  );


        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, cube._vertex_count);

        glBindVertexArray(0);

        render.depth(false);


        //begin 2d
        render.reset();

        // print text
        Font* font = rm()->fonts("font1").get();

        glm::mat4 V1 = glm::mat4(1.0);

        render.set_view_mat(V1);

        
        M = glm::mat4(1.0f);

        render.set_transform(M);
        render.set_color(glm::vec4(1.0, 1.0, 1.0, 1.0));
        render.set_font(font);

        
        render.draw_text(buff, 10.0f, HEIGHT - 40.0f);


       // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
