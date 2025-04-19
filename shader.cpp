
#include <iostream>

#include "shader.h"

char* file_load(const char* filename)
{
    FILE* in = fopen(filename, "rwb");

    if (in == NULL) return NULL;

    int res_size = BUFSIZ;
    char* res = (char*)malloc(res_size);
    int nb_read_total = 0;

    while (!feof(in) && !ferror(in)) {

      if (nb_read_total + BUFSIZ > res_size) {
        if (res_size > 10*1024*1024) break;
        res_size = res_size * 2;
        res = (char*)realloc(res, res_size);
      }

      char* p_res = res + nb_read_total;

      nb_read_total += fread(p_res, 1, BUFSIZ, in);
  }

  fclose(in);

  res = (char*)realloc(res, nb_read_total + 1);
  res[nb_read_total] = '\0';

  return res;
}

GLuint Shader::shader_vertex_load()
{
    GLchar* vertexShaderSource = file_load(_vertex_shader_path.c_str());//"./shader_vertex_01.txt");

    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (success == false)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return vertexShader;
}

GLuint Shader::shader_fragment_load()
{
    GLchar* fragmentShaderSource = file_load(_fragment_shader_path.c_str());//"./shader_fragment.txt");

    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLint success;
    GLchar infoLog[512];

    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (success == false)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return fragmentShader;
}


void Shader::build()
{
    // Build and compile our shader program

    // Vertex shader
    GLuint vertexShader = shader_vertex_load();

    // Fragment shader
    GLuint fragmentShader = shader_fragment_load();


    GLint success;
    GLchar infoLog[512];

    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    _prog_ID = shaderProgram;
}

void Shader::use()
{
    glUseProgram(_prog_ID);
}
