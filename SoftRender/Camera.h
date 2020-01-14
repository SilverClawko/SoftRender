
#ifndef CAMERA_H
#define CAMERA_H

#include "Math.h"
#include "Clip.h"

class Camera
{
public:

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Fov;
	float Aspect;

	float Near;
	float Far;

	float Pitch;
	float Yaw;

	//位置 垂直反转 看的目标点 Fov 宽 高
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3 lookat = glm::vec3(0.0f, 0.0f, 0.0f),
		float fov = 60.0f,
		int w = 800,
		int h = 600,
		float n = 0.3f,
		float f = 100
	):
		Position(position), WorldUp(up),Fov(glm::radians(fov)),Aspect((float)w/h),Pitch(0),Yaw(0),Near(n),Far(f)
	{
		Front = glm::normalize(lookat - Position);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));

		glm::vec3 WorldFront(0, 0, -1);
		glm::vec3 FrontXZ = glm::normalize(glm::vec3(Front.x, 0, Front.z));
		float yd = glm::dot(WorldFront, FrontXZ);
		float pd = glm::dot(Front, FrontXZ);
		if (yd > 1.0)
			yd = 1.0;
		if (yd < -1)
			yd = -1.0;
		if (pd > 1.0)
			pd = 1.0;
		if (pd < -1)
			pd = -1.0;
		Yaw = glm::degrees(acos(yd));
		Pitch = glm::degrees(acos(pd));
	}
	glm::mat4 ViewMatrix()
	{	
		return GetViewMatrix(Position, Front, Right, Up);
	}
	glm::mat4 PerspectiveMatrix() 
	{
		return GetPerspectiveMatrix(Fov,Aspect,Near,Far);
	}
	void UpdateFov(float fov = 60.0f) {
		Fov = glm::radians(fov);
	}
	void UpdateAspect(int w, int h) {
		Aspect = (float)w / h;
	}
	void MoveForward(float distance) {
		Position += Front * distance;
	}
	void MoveRight(float distance) {
		Position += Right * distance;
	}
	void MoveUp(float distance) {
		Position += Up * distance;
	}
	void RotatePitch(float angle) {
		Pitch += angle;
		if (Pitch > 89.0)
			Pitch = 89.0;
		if (Pitch < -89.0)
			Pitch = -89.0;
		UpdateCameraVectors();
	}
	void RotateYaw(float angle) {
		Yaw += angle;
		if (Yaw > 360)
			Yaw = 0;
		if (Yaw < 0)
			Yaw = 360;
		UpdateCameraVectors();
	}
private:
	void UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = -sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = -cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif