#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>

#include <string>

#include "glm_includes.h"

class Shader
{
public:

    Shader()
    {}

    virtual ~Shader()
    {}

    void set_vertex_shader_path(const std::string& path) {
      _vertex_shader_path = path;
    }

    void set_fragment_shader_path(const std::string& path) {
      _fragment_shader_path = path;
    }

    /// set up matrixies Model M, View V, Porijection P
    void set(const std::string& name, const glm::mat4& M)
    {
        GLint uloc = glGetUniformLocation (_prog_ID, name.c_str());

        glUniformMatrix4fv (uloc, 1, GL_FALSE, glm::value_ptr (M));
    }

    // uniform color and others variable
    void set(const std::string& name, const glm::vec4& vec )
    {
        GLint uloc = glGetUniformLocation (_prog_ID, name.c_str());

        glUniform4f(uloc, vec.r, vec.g, vec.b, 1.0);
    }

    void set(const std::string& name, int id)
	{
        GLint uloc = glGetUniformLocation (_prog_ID, name.c_str());

		glUniform1i ( uloc, id );
	}

    void set(const std::string& name, float value)
	{
        GLint uloc = glGetUniformLocation (_prog_ID, name.c_str());

		glUniform1f ( uloc, value );
	}

    void build();
    void use();

private:
    GLuint shader_vertex_load();
    GLuint shader_fragment_load();

    GLuint _prog_ID;

    std::string _vertex_shader_path;
    std::string _fragment_shader_path;
};

#endif //SHADER_H_
