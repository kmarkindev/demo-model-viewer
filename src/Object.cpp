#include "Object.h"

glm::mat4 Object::GetModelMatrix()
{
	auto scale = glm::scale(glm::mat4(1.f), m_scale);
	auto rotate = glm::toMat4(m_rotation);
	auto translate = glm::translate(glm::mat4(1.f), m_position);

	return translate * rotate * scale;
}

glm::vec3 Object::GetForwardVector()
{
	return glm::toMat4(m_rotation) * glm::vec4(0.f, 0.f, 1.f, 0.f);
}

glm::vec3 Object::GetUpVector()
{
	return glm::toMat4(m_rotation) * glm::vec4(0.f, 1.f, 0.f, 0.f);
}

glm::vec3 Object::GetRightVector()
{
	return glm::toMat4(m_rotation) * glm::vec4(1.f, 0.f, 0.f, 0.f);
}

void Object::SetRotation(glm::quat rotation)
{
	m_rotation = rotation;
}

void Object::SetRotationByVector(glm::vec3 rotation)
{
	rotation = glm::radians(rotation);

	m_rotation = glm::angleAxis(rotation.x, glm::vec3(1.f, 0.f, 0.f))
		* glm::angleAxis(rotation.y, glm::vec3(0.f, 1.f, 0.f))
		* glm::angleAxis(rotation.z, glm::vec3(0.f, 0.f, 1.f));
}

glm::quat Object::GetRotation()
{
	return m_rotation;
}

void Object::SetPosition(glm::vec3 newPosition)
{
	m_position = newPosition;
}

glm::vec3 Object::GetPosition()
{
	return m_position;
}

void Object::SetScale(glm::vec3 newScale)
{
	m_scale = newScale;
}

glm::vec3 Object::GetScale()
{
	return m_scale;
}

void Object::Scale(glm::vec3 scale)
{
	m_scale *= scale;
}

void Object::Move(glm::vec3 delta)
{
	m_position += delta;
}

void Object::Rotate(glm::quat rotation)
{
	m_rotation *= rotation;
}

void Object::RotateLocalByVector(glm::vec3 rotation)
{
	rotation = glm::radians(rotation);

	m_rotation = glm::angleAxis(rotation.x, glm::vec3(1.f, 0.f, 0.f))
		* glm::angleAxis(rotation.y, glm::vec3(0.f, 1.f, 0.f))
		* glm::angleAxis(rotation.z, glm::vec3(0.f, 0.f, 1.f))
		* m_rotation;
}

void Object::RotateAround(glm::vec3 origin, glm::vec3 axis, float angle) 
{
	auto position = m_position - origin;

	position = glm::rotate(glm::angleAxis(glm::radians(angle), axis), position);

	m_position = position + origin;
}

void Object::RotateByVector(glm::vec3 rotation)
{
	rotation = glm::radians(rotation);

	m_rotation *= glm::angleAxis(rotation.x, glm::vec3(1.f, 0.f, 0.f))
		* glm::angleAxis(rotation.y, glm::vec3(0.f, 1.f, 0.f))
		* glm::angleAxis(rotation.z, glm::vec3(0.f, 0.f, 1.f));
}

void Object::RotateToDirection(glm::vec3 direction)
{
	auto currentDir = glm::normalize(GetForwardVector());
	direction = glm::normalize(direction);

	float angle = glm::acos(glm::dot(currentDir, direction));
	glm::vec3 axis = glm::normalize(glm::cross(currentDir, direction));

	m_rotation = glm::angleAxis(angle, axis) * m_rotation;
}

Object::Object()
{
	SetRotation(glm::quat(1.f, 0.f, 0.f, 0.f));
	SetPosition(glm::vec3(0.f, 0.f, 0.f));
	SetScale(glm::vec3(1.f, 1.f, 1.f));
}
