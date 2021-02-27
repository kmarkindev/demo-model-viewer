#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Renderer;

class Object
{
private:

	friend class Renderer;

	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_scale;

public:

	Object();

	glm::mat4 GetModelMatrix();

	glm::vec3 GetForwardVector();
	glm::vec3 GetUpVector();
	glm::vec3 GetRightVector();

	void SetRotation(glm::quat rotation);
	void SetRotationByVector(glm::vec3 rotation);
	glm::quat GetRotation();

	void SetPosition(glm::vec3 newPosition);
	glm::vec3 GetPosition();

	void SetScale(glm::vec3 newScale);
	glm::vec3 GetScale();

	void Scale(glm::vec3 scale);
	void Move(glm::vec3 delta);
	void Rotate(glm::quat rotation);
	void RotateByVector(glm::vec3 rotation);
	void RotateToDirection(glm::vec3 direction);
	void RotateLocalByVector(glm::vec3 rotation);
	void RotateAround(glm::vec3 origin, glm::vec3 axis, float angle);

};