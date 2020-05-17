#include "Light.h"
#include "ShaderBase.h"
#include "Texture.h"
#include "Transform.h"

Light::Light(const glm::vec3 & pos, const glm::vec3 & dir, const glm::vec4 & c, float i) :
	position(pos),
	direction(glm::normalize(dir)),
	color(c),
	intensity(i),
	shadowMap(nullptr)
{
	if (direction == glm::vec3(0, -1, 0)) {
		viewMatrix = Transform::GetViewMatrix(pos, direction, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	}
	else if (direction == glm::vec3(0, 1, 0)) {
		viewMatrix = Transform::GetViewMatrix(pos, direction, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else {
		glm::vec3 right = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
		glm::vec3 up = glm::normalize(glm::cross(right, direction));
		viewMatrix = Transform::GetViewMatrix(pos, direction, right, up);
	}
}
Light::~Light()
{
}

void Light::SetShadowMap(Texture2D * t)
{
	if (shadowMap)
		delete shadowMap;
	shadowMap = t;
}

Texture2D * Light::ShadowMap() const
{
	return shadowMap;
}

void Light::SetPosition(const glm::vec3 & pos)
{
	position = pos;
}

void Light::SetDirection(const glm::vec3 & dir)
{
	direction = dir;
}


DirectionLight::DirectionLight(const glm::vec3 & dir, const glm::vec4 & c, float i) :
	Light(glm::vec3(0.0f) - 3.0f * dir, dir, c, i)
{
	projectorMatrix = Transform::GetOrthoMatrix(6.0f, 6.0f, 0.3f, 8.0f);
	lightMatrix = projectorMatrix * viewMatrix;
}

void DirectionLight::SetDirection(const glm::vec3 & dir)
{
	direction = glm::normalize(dir);
	position = glm::vec3(0.0f) - direction * 3.0f;
	if (direction == glm::vec3(0, -1, 0)) {
		viewMatrix = Transform::GetViewMatrix(position, direction, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	}
	else if (direction == glm::vec3(0, 1, 0)) {
		viewMatrix = Transform::GetViewMatrix(position, direction, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else {
		glm::vec3 right = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
		glm::vec3 up = glm::normalize(glm::cross(right, direction));
		viewMatrix = Transform::GetViewMatrix(position, direction, right, up);
	}
	lightMatrix = projectorMatrix * viewMatrix;
}
