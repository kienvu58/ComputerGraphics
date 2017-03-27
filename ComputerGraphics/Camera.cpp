#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>

Camera::Camera() :
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_position(glm::vec3(0.0f, 0.0f, 10.0f)),
    m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_yaw(-90.0f),
    m_pitch(0.0f),
    m_movementSpeed(3.0f),
    m_mouseSensitivity(0.25f),
    m_fov(45.0f)
{
    UpdateCameraVectors();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

float Camera::GetFOV() const
{
    return glm::radians(m_fov);
}

void Camera::Move(CameraMovement direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    switch (direction)
    {
    case CameraMovement::FORWARD:
        m_position += m_front * velocity;
        break;
    case CameraMovement::BACKWARD:
        m_position -= m_front * velocity;
        break;
    case CameraMovement::LEFT:
        m_position -= m_right * velocity;
        break;
    case CameraMovement::RIGHT:
        m_position += m_right * velocity;
        break;
    default:
        break;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;
    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
        {
            m_pitch = 89.0f;
        }
        if (m_pitch < -89.0f)
        {
            m_pitch = -89.0f;
        }
    }
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (m_fov >= 1.0f && m_fov <= 45.0f)
    {
        m_fov -= yoffset;
    }
    if (m_fov < 1.0f)
    {
        m_fov = 1.0f;
    }
    if (m_fov > 45.0f)
    {
        m_fov = 45.0f;
    }
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cosf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch));
    front.y = sinf(glm::radians(m_pitch));
    front.z = sinf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
