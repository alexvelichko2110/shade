#ifndef RECT_RENDER_H_
#define RECT_RENDER_H_

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "glm_includes.h"

#include "texture.h"
#include "shader.h"


class RectRender
{
public:

    static const int MAX_VERTEX_COUNT = 10;

	void init()
	{
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*5*MAX_VERTEX_COUNT, nullptr, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);


        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // // TexCoord attribute
        // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        // glEnableVertexAttribArray(2);

        glBindVertexArray(0); // Unbind VAO
	}

	void draw(Shader& shader, float xc, float yc, float width, float height, glm::vec4& color)
	{
		shader.set("ourColor", color);

		glBindVertexArray(VAO);

        // Set up vertex data (and buffer(s)) and attribute pointers
        GLfloat vertices[] = {
            // Positions         // Texture Coords
            xc + 0.5f*width, yc + 0.5f*height, 0.0f,  1.0f, 1.0f, // Top Right
            xc + 0.5f*width, yc - 0.5f*height, 0.0f,  1.0f, 0.0f, // Bottom Right
            xc - 0.5f*width, yc - 0.5f*height, 0.0f,  0.0f, 0.0f, // Bottom Left
            xc - 0.5f*width, yc + 0.5f*height, 0.0f,  0.0f, 1.0f  // Top Left
        };

        GLuint indices[] = {  // Note that we start from 0!
            0, 1, 3, // First Triangle
            1, 2, 3  // Second Triangle
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4*sizeof(GLfloat)*5, vertices);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*sizeof(GLuint), indices);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);
	}

    void draw_poly(Shader& shader, std::vector<glm::vec3>& points, glm::vec4& color)
	{
		shader.set("ourColor", color);

        
        int size_vertices_array = 5*sizeof(GLfloat)* points.size();

		GLfloat *vertices = (GLfloat*)malloc(size_vertices_array);

        GLuint* indices = (GLuint*)malloc(3*(points.size() - 2)* sizeof(GLuint));

        int index = 0;

		for (unsigned int ind = 0; ind < points.size() - 2; ind++)
		{
			glm::vec3 vec0 = points[0];

			*(vertices + 0) = vec0.x;
			*(vertices + 1) = vec0.y;
			*(vertices + 2) = vec0.z;
            *(vertices + 3) = 0.0;
            *(vertices + 4) = 0.0;

			glm::vec3 vec1 = points[ind + 1];

			*(vertices + (ind + 1)*5 + 0) = vec1.x;
			*(vertices + (ind + 1)*5 + 1) = vec1.y;
			*(vertices + (ind + 1)*5 + 2) = vec1.z;
            *(vertices + (ind + 1)*5 + 3) = 0.0;
            *(vertices + (ind + 1)*5 + 4) = 0.0;

			glm::vec3 vec2 = points[ind + 2];

			*(vertices + (ind + 2)*5 + 0) = vec2.x;
			*(vertices + (ind + 2)*5 + 1) = vec2.y;
			*(vertices + (ind + 2)*5 + 2) = vec2.z;
            *(vertices + (ind + 2)*5 + 3) = 0.0;
            *(vertices + (ind + 2)*5 + 4) = 0.0;

            indices[index + 0] = 0;
            indices[index + 1] = ind + 1;
            indices[index + 2] = ind + 2;

            index += 3;
		}


		glBindVertexArray(VAO);

        // // Set up vertex data (and buffer(s)) and attribute pointers
        // GLfloat vertices[] = {
        //     // Positions         // Texture Coords
        //     xc + 0.5f*width, yc + 0.5f*height, 0.0f,  1.0f, 1.0f, // Top Right
        //     xc + 0.5f*width, yc - 0.5f*height, 0.0f,  1.0f, 0.0f, // Bottom Right
        //     xc - 0.5f*width, yc - 0.5f*height, 0.0f,  0.0f, 0.0f, // Bottom Left
        //     xc - 0.5f*width, yc + 0.5f*height, 0.0f,  0.0f, 1.0f  // Top Left
        // };

        // GLuint indices[] = {  // Note that we start from 0!
        //     0, 1, 3, // First Triangle
        //     1, 2, 3  // Second Triangle
        // };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size_vertices_array, vertices);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 3*(points.size() - 2)*sizeof(GLuint), indices);

        glDrawElements(GL_TRIANGLES, 3*(points.size() - 2), GL_UNSIGNED_INT, 0);

        // glDrawArrays(GL_POLYGON, 0, points.size());

        
        glBindVertexArray(0);

        free(vertices);
        free(indices);
	}

    void draw(Shader& shader, float xc, float yc, float width, float height)
	{
		// shader.set("alpha", 0.2f);

        // glActiveTexture ( GL_TEXTURE0 );
        // glBindTexture ( GL_TEXTURE_2D, tex->get_texID() );

        // shader.set("ourTexture", 0);


		glBindVertexArray(VAO);

        // Set up vertex data (and buffer(s)) and attribute pointers
        GLfloat vertices[] = {
            // Positions         // Texture Coords
            xc + 0.5f*width, yc + 0.5f*height, 0.0f,  1.0f, 1.0f, // Top Right
            xc + 0.5f*width, yc - 0.5f*height, 0.0f,  1.0f, 0.0f, // Bottom Right
            xc - 0.5f*width, yc - 0.5f*height, 0.0f,  0.0f, 0.0f, // Bottom Left
            xc - 0.5f*width, yc + 0.5f*height, 0.0f,  0.0f, 1.0f  // Top Left
        };

        GLuint indices[] = {  // Note that we start from 0!
            0, 1, 3, // First Triangle
            1, 2, 3  // Second Triangle
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4*sizeof(GLfloat)*5, vertices);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*sizeof(GLuint), indices);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);
	}

	GLuint VAO, VBO, EBO;
};

#endif //RECT_RENDER_H_