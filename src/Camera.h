#pragma once

#include "Object.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/matrix_decompose.hpp>

class Camera : public Object
{
private:

	glm::mat4 m_projection;

public:

	Camera();

	void SetPerspectiveMatrix(float fov, int width, int height, float near, float far);
	void SetOrtographicMatrix(float left, float right, float bottom, float top, float near, float far);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

};