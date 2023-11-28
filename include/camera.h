#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
private:
    void m_update_cam_vectors();
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    float yaw;
    float pitch;

    float move_speed;
    float mouse_sens;
    float zoom;

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);

    glm::mat4 get_view_matrix();
    void process_keyboard(CameraMovement direction, float deltaTime);
    void process_mouse_movement(float xoffset, float yoffset, bool constrainPitch);
    void process_mouse_scroll(float yoffset);
};