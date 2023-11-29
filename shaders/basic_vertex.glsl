#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in uint inUniformID;

out vec2 texCoord;

layout (std140, binding = 0) uniform matrices {
    mat4[3] models;
};

uniform mat4 view;
uniform mat4 proj;

void main() {
    texCoord = inTexCoord;
    gl_Position = proj * view * models[inUniformID] * vec4(inPos, 1.0);
}