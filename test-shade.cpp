
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
GLuint WIDTH = 1024, HEIGHT = 768;


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





glm::mat4 get_mat_node(float x, float y, float angle)
{
    glm::mat4 M = glm::mat4(1.0f);
    M = glm::translate(M, glm::vec3(x, y, 0.0f));
    M = glm::rotate(M, angle, glm::vec3(0.0f, 0.0f, 1.0f));

    return M;
}

// const int max_vertex_count = 10000;
// const int max_index_count = 5000;


// class Mesh
// {
// public:

//     Mesh() {
//         // 5 - vertex : pos 3, tex 2
//         _data_stride = sizeof(GLfloat)*5;

//         _vertex_count = 0;
//         _vertexlist = (unsigned char *)malloc(_data_stride*max_vertex_count);
//         _indexlist = (GLuint *)malloc(sizeof(GLuint)*max_index_count);
//     }

//     ~Mesh() {
//         free(_vertexlist);
//         free(_indexlist);
//     }

//     struct PolyOBJ
//     {
//         int v[3];
//         int t[3];
//     };

//     void load_OBJ(const std::string& path)
//     {
//         std::vector<PolyOBJ> temp_poly;
//         std::vector< glm::vec3 > temp_vertices;
//         std::vector< glm::vec2 > temp_uvs;

//         FILE * file = fopen(path.c_str(), "r");
//         if ( file == NULL ) {
//             printf("Impossible to open the file !\n");
//             return;
//         }

//         while( 1 ){

//             char lineHeader[128];
//             // read the first word of the line
//             int res = fscanf(file, "%s", lineHeader);
//             if (res == EOF)
//                 break; // EOF = End Of File. Quit the loop.

//             // else : parse lineHeader

//             if ( strcmp( lineHeader, "v" ) == 0 ){
//                 glm::vec3 vertex;
//                 fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
//                 temp_vertices.push_back(vertex);
//             }
//             else if ( strcmp( lineHeader, "vt" ) == 0 ){
//                 glm::vec2 uv;
//                 fscanf(file, "%f %f\n", &uv.x, &uv.y );
//                 temp_uvs.push_back(uv);
//             }
//             // else if ( strcmp( lineHeader, "vn" ) == 0 ){
//             //     glm::vec3 normal;
//             //     fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
//             //     temp_normals.push_back(normal);
//             // }
//             else if ( strcmp( lineHeader, "f" ) == 0 ){
                
//                 unsigned int vertexIndex[3], uvIndex[3];
//                 int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2] );
//                 if (matches != 6){
//                     printf("File can't be read by our simple parser : ( Try exporting with other options\n");
//                     return;
//                 }

//                 PolyOBJ poly;

//                 poly.v[0] = vertexIndex[0];
//                 poly.v[1] = vertexIndex[1];
//                 poly.v[2] = vertexIndex[2];

//                 poly.t[0] = uvIndex[0];
//                 poly.t[1] = uvIndex[1];
//                 poly.t[2] = uvIndex[2];

//                 temp_poly.push_back(poly);
                
                
//                 // vertexIndices.push_back(vertexIndex[0]);
//                 // vertexIndices.push_back(vertexIndex[1]);
//                 // vertexIndices.push_back(vertexIndex[2]);
//                 // uvIndices    .push_back(uvIndex[0]);
//                 // uvIndices    .push_back(uvIndex[1]);
//                 // uvIndices    .push_back(uvIndex[2]);
//                 // normalIndices.push_back(normalIndex[0]);
//                 // normalIndices.push_back(normalIndex[1]);
//                 // normalIndices.push_back(normalIndex[2]);
//             }


//         }

        
//         int ind = 0;
//         int tri = 0;

//         GLfloat *vlist = nullptr;

//         // For each vertex of each triangle
//         for( unsigned int i=0; i<temp_poly.size(); i++ ){

//             //индекс позиции вершины - это vertexIndices[i] :
//             int vertexIndex1 = temp_poly[i].v[0];
//             int vertexIndex2 = temp_poly[i].v[1];
//             int vertexIndex3 = temp_poly[i].v[2];

//             int uvIndex1 = temp_poly[i].t[0];
//             int uvIndex2 = temp_poly[i].t[1];
//             int uvIndex3 = temp_poly[i].t[2];

//             glm::vec3 p = temp_vertices[ vertexIndex1-1 ];
//             glm::vec2 t = temp_uvs[ uvIndex1-1 ];

//             // compute memory position of next free vertex
//             vlist = (GLfloat *)(_vertexlist + ind*_data_stride);

//             vlist[0] = p.x;
//             vlist[1] = p.y;
//             vlist[2] = p.z;
//             vlist[3] = t.x;
//             vlist[4] = t.y;

//             ind++;

//             p = temp_vertices[ vertexIndex2-1 ];
//             t = temp_uvs[ uvIndex2-1 ];

//             // compute memory position of next free vertex
//             vlist = (GLfloat *)(_vertexlist + ind*_data_stride);

//             vlist[0] = p.x;
//             vlist[1] = p.y;
//             vlist[2] = p.z;
//             vlist[3] = t.x;
//             vlist[4] = t.y;

//             ind++;

//             p = temp_vertices[ vertexIndex3-1 ];
//             t = temp_uvs[ uvIndex3-1 ];

//             // compute memory position of next free vertex
//             vlist = (GLfloat *)(_vertexlist + ind*_data_stride);

//             vlist[0] = p.x;
//             vlist[1] = p.y;
//             vlist[2] = p.z;
//             vlist[3] = t.x;
//             vlist[4] = t.y;

//             ind++;

//             _indexlist[tri+0] = ind - 3;
//             _indexlist[tri+1] = ind - 2;
//             _indexlist[tri+2] = ind - 1;

//             tri += 3;

//             _vertex_count += 3;
//             _index_count += 3;
//         }

//     }

//     int _data_stride;

//     // data vertex and index
// 	int _vertex_count;
// 	unsigned char *_vertexlist;

// 	int _index_count;
// 	GLuint *_indexlist;
// };

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
    node.set_yaw(60.0);
    node.set_pitch(10.0);

    node.set_position(glm::vec3(0.0, 0.0, 0.0));
    // std::cout << "pos " << std::endl;
    
    // attach camera to its node
    camera.set_node(&node_cam);
    // std::cout << "pos 1" << std::endl;


    // FrameBuffer framebuffer(600, 400);

    Render render;
    render.load_resources_all();

    rm()->load_font("font1", "./resources/RDR.ttf");


    render.set_framebuffer_size(WIDTH, HEIGHT);
    render.init();

    // std::cout << "pos 2 " << std::endl;


    Texture *cube_tex = Texture::load("./resources/tex1.jpeg");

    Mesh cube;

    cube.load_OBJ("../../untitled.obj");

        // std::cout << "pos 3" << std::endl;


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


 
    while ( glfwWindowShouldClose ( window ) != true)
    {

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
        
 
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
        M = node.get_mat_inverse();
        render.set_transform(M);
        
        render.set_color(glm::vec4(1.0, 0.0, 0.0, 1.0));
        render.draw_line(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 100.0, 0.0));

        render.set_color(glm::vec4(0.0, 1.0, 0.0, 1.0));
        render.draw_line(glm::vec3(0.0, 0.0, 0.0), glm::vec3(100.0, 0.0, 0.0));

        render.set_color(glm::vec4(0.0, 0.0, 1.0, 1.0));
        render.draw_line(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 100.0));


        // draw cube
        Shader* shader = rm()->shaders("rect_tex").get();
        		
        shader->use();

        glActiveTexture ( GL_TEXTURE0 );    
        glBindTexture ( GL_TEXTURE_2D, cube_tex->get_texID() );

        shader->set("ourTexture", 0);

        //glm::mat4 M1 = node.get_mat_inverse();//glm::inverse(node.get_mat() * node1.get_mat());
        
        M = glm::scale(M, glm::vec3(20.0));

        shader->set("M", P*V*M);


        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, cube._vertex_count);

        glBindVertexArray(0);

        render.depth(false);

        
        render.reset();

        Font* font = rm()->fonts("font1").get();

        M = glm::mat4(1.0f);
        // M = glm::translate(M, glm::vec3(0.0f, 0.0f, 0.0f));

        render.set_transform(M);
        render.set_color(glm::vec4(1.0, 1.0, 1.0, 1.0));
        render.set_font(font);
        render.draw_text("CUBE", 10.0f, HEIGHT - 40.0f);


        // glm::mat4 mat = get_mat_node(500.0f, 400.0f, 30.0f);

        // render.set_transform(mat);
        // render.set_line_width(1.0);
        // render.set_color(glm::vec4(1.0, 0.0, 0.0, 1.0));

        // render.draw_line(0.0, -100.0, 0.0, 100.0);
        // render.draw_line(-100.0, 0.0, 100.0, 0.0);


        // mat = get_mat_node(700.0f, 300.0f, 0.0f);

        // render.set_transform(mat);

        // render.set_color(glm::vec4(0.0, 1.0, 0.0, 1.0));

        // render.set_line_width(3.0);

        // // render.draw_line(-50.0, -40.0, 20.0, 40.0);

        // std::vector<glm::vec3> points;

        // points.push_back(glm::vec3(-50.0, -50.0, 0.0));
        // points.push_back(glm::vec3(-90.0, 100.0, 0.0));
        // points.push_back(glm::vec3(40.0, 80.0, 0.0));
        // points.push_back(glm::vec3(140.0, -20.0, 0.0));
        // // points.push_back(glm::vec3(-50.0, -50.0, 0.0));

        // render.draw_line_strip(points);
        // // render.draw_poly(points);



       // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
