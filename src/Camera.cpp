#include "Camera.h"

void Camera::SetPerspectiveMatrix(float fov, int width, int height, float near, float far)
{
	m_projection = glm::perspectiveFov(glm::radians(fov), (float)width, (float)height, near, far);
}

void Camera::SetOrtographicMatrix(float left, float right, float bottom, float top, float near, float far)
{
	m_projection = glm::ortho(left, right, bottom, top, near, far);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return m_projection;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(GetPosition(), GetPosition() + GetForwardVector(), GetUpVector());
}

Camera::Camera()
{
}
