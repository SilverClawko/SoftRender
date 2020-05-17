#ifndef CAMERA_H
#define CAMERA_H

#include "Global.h"

class Camera
{
private:

	glm::mat4 viewMatrix;
	glm::mat4 projectMatrix;

	bool viewChanged;
	bool projectionChanged;

public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	float fov;
	float aspect;

	float near;
	float far;

	float pitch;
	float yaw;

	//位置 垂直反转 看的目标点 Fov 宽 高
	Camera(
		int w,
		int h,
		float fv = 60.0f,
		const glm::vec3 &pos = glm::vec3(0.0f, 0.0f, 2.0f),
		const glm::vec3 &u = glm::vec3(0.0f, 1.0f, 0.0f),
		const glm::vec3 &lookat = glm::vec3(0.0f, 0.0f, 0.0f),
		float n = 0.3f,
		float f = 100.0f
	);
	glm::mat4 & GetViewMatrix();
	glm::mat4 & GetProjectionMatrix();

	void UpdateFov(float fv = 60.0f);
	void UpdateAspect(int w, int h);
	void MoveForward(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);
	void RotatePitch(float angle);
	void RotateYaw(float angle);

private:
	void UpdateCameraVectors();
};

#endif

