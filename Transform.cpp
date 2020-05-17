#include "Transform.h"

glm::mat4 Transform::GetViewPortMatrix(int ox, int oy, int width, int height)
{
	glm::mat4 result = glm::mat4(1.0f);
	result[0][0] = width / 2.0f;
	result[3][0] = ox + (width / 2.0f);
	result[1][1] = height / 2.0f;
	result[3][1] = oy + (height / 2.0f);
	return result;
}

glm::mat4 Transform::GetViewMatrix(const glm::vec3 &pos, const glm::vec3 & front, const glm::vec3 & right, const glm::vec3 & up)
{
	glm::mat4 result = glm::mat4(1.0f);
	result[0][0] = right.x;
	result[1][0] = right.y;
	result[2][0] = right.z;
	result[3][0] = -glm::dot(right, pos);
	result[0][1] = up.x;
	result[1][1] = up.y;
	result[2][1] = up.z;
	result[3][1] = -glm::dot(up, pos);
	result[0][2] = -front.x;
	result[1][2] = -front.y;
	result[2][2] = -front.z;
	result[3][2] = glm::dot(front, pos);
	return result;
}

glm::mat4 Transform::GetPerspectiveMatrix(float fovy, float aspect, float n, float f)
{
	glm::mat4 result = glm::mat4(0.0f);
	const float tanHalfFov = tan(fovy*0.5f);
	result[0][0] = 1.0f / (aspect*tanHalfFov);
	result[1][1] = 1.0f / (tanHalfFov);
	result[2][2] = -(f + n) / (f - n);
	result[2][3] = -1.0f;
	result[3][2] = (-2.0f * n * f) / (f - n);
	return result;
}

glm::mat4 Transform::GetOrthoMatrix(int w, int h, float n, float f)
{
	glm::mat4 result = glm::mat4(1.0f);
	result[0][0] = 2.0f / w;
	result[1][1] = 2.0f / h;
	result[2][2] = -2.0f / (f - n);
	result[3][2] = -(f + n) / (f - n);
	return result;
}

