#include "shader.h"
#include "file_io.h"
#include <glad/glad.h>

#include <fstream>
#include <iostream>

Shader::Shader() : m_handle(-1) {  }

void Shader::load_from_file(const std::string& vertex_path, const std::string& fragment_path) {
    std::string vertex_source;
    if (!read_text_file_into_string(vertex_path, vertex_source)) {
        std::cerr << "Failed to open vertex source file\n";
    }
    
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    auto temp = vertex_source.c_str();
    glShaderSource(vertex_shader, 1, &temp, nullptr);
    glCompileShader(vertex_shader);

    int success;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int length = 0;
        char buffer[1024];
        glGetShaderInfoLog(vertex_shader, 1024, &length, buffer);
        std::cerr << buffer << "\n";
    }

    std::string fragment_source;
    if (!read_text_file_into_string(fragment_path, fragment_source)) {
        std::cerr << "Failed to open fragment source file\n";
    }

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    temp = fragment_source.c_str();
    glShaderSource(fragment_shader, 1, &temp, nullptr);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int length = 0;
        char buffer[1024];
        glGetShaderInfoLog(fragment_shader, 1024, &length, buffer);
        std::cerr << buffer << "\n";
    }

    m_handle = glCreateProgram();
    glAttachShader(m_handle, vertex_shader);
    glAttachShader(m_handle, fragment_shader);
    glLinkProgram(m_handle);

    glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
    if (!success) {
        int length = 0;
        char buffer[1024];
        glGetProgramInfoLog(m_handle, 1024, &length, buffer);
        std::cerr << buffer << "\n";
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

unsigned int Shader::get_handle() {
    return m_handle;
}


