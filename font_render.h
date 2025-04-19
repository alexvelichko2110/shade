#ifndef FONT_RENDER_H_
#define FONT_RENDER_H_

// Std. Includes
#include <iostream>
#include <map>
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "glm_includes.h"

#include "shader.h"
#include "font.h"

class FontRender {
public:

	void init()
    {
	    // Configure VAO/VBO for texture quads
	    glGenVertexArrays(1, &VAO);
	    glGenBuffers(1, &VBO);

	    glBindVertexArray(VAO);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	    glEnableVertexAttribArray(0);

	    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		
	    glBindBuffer(GL_ARRAY_BUFFER, 0);
	    glBindVertexArray(0);
	}

	void draw(Shader& shader, Font& font, const char* text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec4& color)
	{
	    // Activate corresponding render state

		shader.set("textColor", color);


	    glActiveTexture(GL_TEXTURE0);

	    // Render glyph texture over quad
	    glBindTexture(GL_TEXTURE_2D, font.get_texture_ID());

    	shader.set("text", 0);
	    

		glBindVertexArray(VAO);


		const char* temp = text;

	    // Iterate through all characters
	    while( *temp )
	    {
	    	unsigned char c = *temp;// + 48;

	    	if (c <= 127)
	    		temp += 1;
	    	else {
	    		c = *(temp + 1) + 48;
	    		temp += 2;
	    	}

	    	Character ch = font.get_glyph(c);//Characters[c];

	//		printf("с %d tx %d ty %d \n", c, ch.tx, ch.ty);

	        GLfloat xpos = x + ch.Bearing.x * scale;
	        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

	        GLfloat w = ch.Size.x * scale;
	        GLfloat h = ch.Size.y * scale;

			GLfloat tx = ch.tx;
			GLfloat ty = ch.ty;

			GLfloat sx = ch.Size.x / (GLfloat)font.get_width();
			GLfloat sy = ch.Size.y / (GLfloat)font.get_height();

	        // Update VBO for each character
	        GLfloat vertices[6][4] = {
	            { xpos,     ypos + h,   tx, ty },
	            { xpos,     ypos,       tx, ty+sy },
	            { xpos + w, ypos,       tx+sx, ty+sy },

	            { xpos,     ypos + h,   tx,ty },
	            { xpos + w, ypos,       tx+sx, ty+sy },
	            { xpos + w, ypos + h,   tx+sx, ty }
	        };

	        // Update content of VBO memory
	        glBindBuffer(GL_ARRAY_BUFFER, VBO);
	        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

	        glBindBuffer(GL_ARRAY_BUFFER, 0);
	        // Render quad
	        glDrawArrays(GL_TRIANGLES, 0, 6);

	        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
	        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	    }

	    glBindVertexArray(0);
	    glBindTexture(GL_TEXTURE_2D, 0);
	}

private:
	GLuint VAO, VBO;
};

#endif // FONT_RENDER_H_
