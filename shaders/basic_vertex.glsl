#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    texCoord = inTexCoord;
    gl_Position = proj * view * model * vec4(inPos, 1.0);
}