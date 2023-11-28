#include "camera.h"

#include <glad/glad.h>
#include <vector>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

Camera::Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), move_speed(SPEED), mouse_sens(SENSITIVITY), zoom(ZOOM) {
    this->position = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;
    m_update_cam_vectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), move_speed(SPEED), mouse_sens(SENSITIVITY), zoom(ZOOM) {
    this->position = glm::vec3(posX, posY, posZ);
    this->world_up = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;
    m_update_cam_vectors();
}

glm::mat4 Camera::get_view_matrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::process_keyboard(CameraMovement direction, float deltaTime) {
    float velocity = move_speed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}

void Camera::process_mouse_movement(float xoffset, float yoffset, bool constrainPitch = true) {
    xoffset *= mouse_sens;
    yoffset *= mouse_sens;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    m_update_cam_vectors();
}

void Camera::process_mouse_scroll(float yoffset) {
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::m_update_cam_vectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}