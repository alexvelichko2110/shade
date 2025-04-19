#ifndef LINE_RENDER_H_
#define LINE_RENDER_H_

#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "glm_includes.h"

#include "shader.h"

class LineRender
{
public:

	static const int MAX_VERTEX_COUNT = 10;

	void init()
	{
		// GLuint VAO;
		glGenVertexArrays(1, &VAO);

		// GLuint VBO;
		glGenBuffers(1, &VBO);


		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * MAX_VERTEX_COUNT * 3, nullptr, GL_DYNAMIC_DRAW);


		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)0);
		glEnableVertexAttribArray(0);
		
		// // Position attribute
		// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, _data_stride, (GLvoid*)(3 * sizeof(GLfloat)));
		// glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	    glBindVertexArray(0);
	}

	void draw(Shader& shader, float xa, float ya, float xb, float yb, glm::vec4& color, float line_width)
	{
		shader.set("ourColor", color);

		glLineWidth(line_width);

		glBindVertexArray(VAO);

		// Update VBO for each character
		GLfloat vertices[2][3] = {
			{ xa,  ya, 0.0 },
			{ xb,  yb, 0.0 },
		};

		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Render line
		glDrawArrays(GL_LINES, 0, 2);
	}

	void draw(Shader& shader, std::vector<glm::vec3>& points, glm::vec4& color, float line_width)
	{
		shader.set("ourColor", color);

		glLineWidth(line_width);

		glBindVertexArray(VAO);

		// std::cout << "points count " << points.size() << std::endl;

		int size_vertices_array = 3*sizeof(GLfloat)* points.size();

		GLfloat *vertices = (GLfloat*)malloc(size_vertices_array);

		for (unsigned int ind = 0; ind < points.size(); ind++)
		{
			glm::vec3 vec = points[ind];

			*(vertices + ind*3 + 0) = vec.x;
			*(vertices + ind*3 + 1) = vec.y;
			*(vertices + ind*3 + 2) = vec.z;
		}

		// // Update VBO for each character
		// GLfloat vertices[2][3] = {
		// 	{ xa,  ya, 0.0 },
		// 	{ xb,  yb, 0.0 },
		// };

		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size_vertices_array, vertices);

		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Render line
		glDrawArrays(GL_LINE_STRIP, 0, points.size());

		free(vertices);
	}

	GLuint VAO, VBO;
};

#endif //LINE_RENDER_H_
