#include <iostream>

#include "render.h"

// void Render::draw_buffer(Buffer* buffer, bool blend_enable)
// {    
//     if (blend_enable)
//         blend(true);


//     buffer->enable(true);

//     int index_count = buffer->get_index_count();
//     int elem_type  = buffer->get_element_type();

//     // std::cout << " index count " << index_count << std::endl;

//     glDrawElements(elem_type, index_count, GL_UNSIGNED_INT, 0);


//     buffer->enable(false);

//     if (blend_enable)
//         blend(false);
// }

void Render::draw_buffer_u(Buffer* buffer, Node* node, Camera* camera, bool blend_enable)
{
    //std::cout << " iter4" << std::endl;

    Shader* shader = nullptr;
    
    if (buffer->get_element_type() == GL_TRIANGLES)
        shader = rm()->shaders("poly").get();
    else
    if (buffer->get_element_type() == GL_LINES)
        shader = rm()->shaders("line1").get();

    // Be sure to activate the shader
    shader->use();

    glm::mat4 P = camera->get_proj_matrix();
    glm::mat4 V = camera->get_node()->get_mat();

    glm::mat4 M = node->get_mat_inverse();
    //std::cout << " iter5" << std::endl;
    // glm::mat4 V = glm::mat4(1.0f);
    // glm::mat4 M = glm::mat4(1.0f);

    glm::mat4 mat = P*V*M;

    shader->set("M", mat);

    
    // Texture* tex = rm()->textures("default").get();

    // // tex->bind();

    // glActiveTexture ( GL_TEXTURE0 );
    // glBindTexture ( GL_TEXTURE_2D, tex->get_texID() );


    shader->set("ourTexture", 0);

    
    shader->set("ourColor", glm::vec4(0.3, 0.3, 0.3, 1.0));



    if (blend_enable)
        blend(true);

    // drwaing line strip
    buffer->enable(true);

    int index_count = buffer->get_index_count();
    int elem_type  = buffer->get_element_type();

    // std::cout << " index count " << index_count << std::endl;

    glDrawElements(elem_type, index_count, GL_UNSIGNED_INT, 0);


    buffer->enable(false);

    if (blend_enable)
        blend(false);
}

