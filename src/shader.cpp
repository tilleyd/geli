#include <geli/shader.hpp>

#include <stdexcept>

#include <GL/glew.h>

using namespace geli;

Shader::Shader(const std::string& vs, const std::string& fs) :
    _shader(0)
{
    // compile the shaders
    GLuint vert = _compile_shader(vs.c_str(), GL_VERTEX_SHADER);
    GLuint frag = _compile_shader(fs.c_str(), GL_FRAGMENT_SHADER);
    // and link
    _shader = _link_program(vert, frag);
}

Shader::~Shader()
{
    if (_shader) {
        glDeleteProgram(_shader);
    }
}

void Shader::use() const
{
    glUseProgram(_shader);
}

unsigned int Shader::_link_program(unsigned int vert, unsigned int frag)
{
    GLuint program = glCreateProgram();
    // link the program and get the result
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    GLint result = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    // clean up the shaders before possibly throwing an exception
    glDetachShader(program, vert);
    glDetachShader(program, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);

    if (result == GL_FALSE) {
        throw std::runtime_error("failed to link vertex and fragment shaders");
    }
    return program;
}

unsigned int Shader::_compile_shader(const char* glsl, unsigned int type)
{
    GLuint shader = glCreateShader(type);
    // compile the shader
    glShaderSource(shader, 1, &glsl, nullptr);
    glCompileShader(shader);
    // check the results
    GLint result = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        glDeleteShader(shader);
        throw std::runtime_error("failed to compile shader");
    }
    return shader;
}