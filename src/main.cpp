#include <SFML/Window.hpp>
#include <glad/glad.h>

#include <fstream>
#include <iostream>

#include "shader.h"

void GLAPIENTRY glErrorCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw

        window.display();
    }

    return 0;
}