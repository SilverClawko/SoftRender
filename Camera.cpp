#include "Camera.h"
#include "Transform.h"
#include "Clip.h"
#include "Function.h"
//位置 垂直反转 看的目标点 Fov 宽 高
Camera::Camera(
	int w,
	int h,
	float fv,
	const glm::vec3 &pos,
	const glm::vec3 &u,
	const glm::vec3 &lookat,
	float n,
	float f
)
	:
	position(pos),
	worldUp(u),
	fov(glm::radians(fv)),
	aspect((float)w / h),
	near(n),
	far(f),
	pitch(0),
	yaw(0),
	viewChanged(false),
	projectionChanged(false),
	viewMatrix(1.0f),
	projectMatrix(1.0f)
{
	front = glm::normalize(lookat - position);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
	glm::vec3 euler = Dir2Euler(front);
	pitch = euler.x;
	yaw = euler.y;

	viewMatrix = Transform::GetViewMatrix(position, front, right, up);
	//projectMatrix = Transform::GetOrthoMatrix(6.0f, 6.0f, 0.3f, 8.0f);
	projectMatrix = Transform::GetPerspectiveMatrix(fov, aspect, near, far);

	//set clip
	Clip::near = near;
	Clip::far = far;
}

glm::mat4 & Camera::GetViewMatrix()
{
	if (viewChanged) {
		viewChanged = false;
		UpdateCameraVectors();
		viewMatrix = Transform::GetViewMatrix(position, front, right, up);
	}
	return viewMatrix;
}

glm::mat4 & Camera::GetProjectionMatrix()
{
	if (projectionChanged) {
		projectionChanged = false;
		//projectMatrix = Transform::GetOrthoMatrix(6.0f, 6.0f, 0.3f, 8.0f);
		projectMatrix = Transform::GetPerspectiveMatrix(fov, aspect, near, far);
	}
	return projectMatrix;
}

void Camera::UpdateFov(float fv) {
	fov = glm::radians(fv);
	if (fov > 1.5707964) 
		fov = 1.5707964;
	if (fov < 0.7853982)
		fov = 0.7853982;
	projectionChanged = true;
}

void Camera::UpdateAspect(int w, int h) {
	aspect = (float)w / h;
	projectionChanged = true;
}

void Camera::MoveForward(float distance) {
	position += front * distance;
	viewChanged = true;
}

void Camera::MoveRight(float distance) {
	position += right * distance;
	viewChanged = true;
}

void Camera::MoveUp(float distance) {
	position += up * distance;
	viewChanged = true;
}

void Camera::RotatePitch(float angle) {
	pitch += angle;
	if (pitch > 89.0)
		pitch = 89.0;
	if (pitch < -89.0)
		pitch = -89.0;
	//UpdateCameraVectors();
	viewChanged = true;
}

void Camera::RotateYaw(float angle) {
	yaw += angle;
	if (yaw > 360)
		yaw = 0;
	if (yaw < 0)
		yaw = 360;
	//UpdateCameraVectors();
	viewChanged = true;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 _front = Euler2Dir(glm::vec3(pitch, yaw, 0.0f));
	//_front.x = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//_front.y = sin(glm::radians(pitch));
	//_front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(_front);
	right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}
