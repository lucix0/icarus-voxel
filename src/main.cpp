#include <SFML/Window.hpp>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>

#include "shader.h"
#include "camera.h"
#include "Texture.h"

// 24 bytes
struct Vertex {
    float x;
    float y;
    float z;
    float u;
    float v;
    unsigned int uniform_id;
};

Vertex points[] = {
    {  0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 0 },
    {  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0 },
    { -0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0 },

    {  0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1 },
    {  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1 },
    { -0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 1 },

    {  0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 2 },
    {  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 2 },
    { -0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 2 }
};

glm::mat4 model_uniforms[] = {
    glm::mat4(1.0f),
    glm::mat4(1.0f),
    glm::mat4(1.0f)
};

void glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param) {
    std::string const src_str = [source]() {
        switch (source) {
            case GL_DEBUG_SOURCE_API: return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        }
    }();

    std::string const type_str = [type]() {
        switch (type) {
            case GL_DEBUG_TYPE_ERROR: return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER: return "MARKER";
            case GL_DEBUG_TYPE_OTHER: return "OTHER";
        }
    }();

    std::string const severity_str = [severity]() {
        switch (severity) {
            case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
            case GL_DEBUG_SEVERITY_LOW: return "LOW";
            case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
            case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        }
    }();
    std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 4;
    settings.minorVersion = 6;
    settings.attributeFlags = settings.Core;

    sf::Window window(sf::VideoMode(800, 600), "icarus", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    gladLoadGL();

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glErrorCallback, 0);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.5, 0.25, 1.0, 1.0);

    Shader test_shader;
    test_shader.load_from_file("shaders/basic_vertex.glsl", "shaders/basic_fragment.glsl");
    glUseProgram(test_shader.get_handle());

    unsigned int vertex_buffer;
    glCreateBuffers(1, &vertex_buffer);
    glNamedBufferData(vertex_buffer, 216, &points, GL_STATIC_DRAW);

    unsigned int vertex_array;
    glCreateVertexArrays(1, &vertex_array);
    glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, 24);

    glEnableVertexArrayAttrib(vertex_array, 0);
    glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vertex_array, 0, 0);

    glEnableVertexArrayAttrib(vertex_array, 1);
    glVertexArrayAttribFormat(vertex_array, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(vertex_array, 1, 0);

    glEnableVertexArrayAttrib(vertex_array, 2);
    glVertexArrayAttribIFormat(vertex_array, 2, 1, GL_UNSIGNED_INT, 5 * sizeof(float));
    glVertexArrayAttribBinding(vertex_array, 2, 0);

    glBindVertexArray(vertex_array);

    model_uniforms[0] = glm::translate(model_uniforms[0], { 0.0f, 1.0f, 0.0f });
    model_uniforms[1] = glm::translate(model_uniforms[0], { 0.0f, -1.0f, 0.0f });
    model_uniforms[2] = glm::translate(model_uniforms[0], { -1.0f, 1.0f, 0.0f });

    unsigned int modelUbo;
    glCreateBuffers(1, &modelUbo);
    glNamedBufferData(modelUbo, 192, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, modelUbo);
    glNamedBufferSubData(modelUbo, 0, 192, &model_uniforms);

    Camera cam(0.0f, 0.0f, 3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 45.0f);
    glm::mat4 proj = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    Texture test_texture;
    test_texture.load_from_file("resources/container.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, test_texture.get_handle());

    bool shouldQuit = false;
    while (!shouldQuit) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    shouldQuit = true;
                    break;
                case sf::Event::Resized:
                    glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        cam.process_keyboard(BACKWARD, 0.01f);

        int proj_loc = glGetUniformLocation(test_shader.get_handle(), "proj");
        glUniformMatrix4fv(proj_loc, 1, false, glm::value_ptr(proj));

        int view_loc = glGetUniformLocation(test_shader.get_handle(), "view");
        glUniformMatrix4fv(view_loc, 1, false, glm::value_ptr(cam.get_view_matrix()));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int count[3] = { 3, 3, 3 };
        int first[3] = { 0, 3, 6 };
        glMultiDrawArrays(GL_TRIANGLES, first, count, 3);

        window.display();
    }

    return 0;
}