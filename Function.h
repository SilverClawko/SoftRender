#ifndef MATH_H
#define MATH_H
#include "Global.h"
#include "Vertex.h"

const float PI = 3.14159265359;

static glm::vec2 min(glm::vec2 v1,glm::vec2 v2) {
	glm::vec2 res;
	res.x = v1.x > v2.x ? v2.x : v1.x;
	res.y = v1.y > v2.y ? v2.y : v1.y;
	return res;
}
static glm::vec2 max(glm::vec2 v1, glm::vec2 v2) {
	glm::vec2 res;
	res.x = v1.x > v2.x ? v1.x : v2.x;
	res.y = v1.y > v2.y ? v1.y : v2.y;
	return res;
}
static int min(int  a, int b) {
	return a > b ? b : a;
}
static int max(int  a, int b) {
	return a > b ? a : b;
}
static float min(float  a, float b) {
	return a > b ? b : a;
}
static float max(float  a, float b) {
	return a > b ? a : b;
}

static inline glm::vec4 lerp(const glm::vec4 &v1, const glm::vec4 &v2, float factor) {
	return (1.0f - factor) * v1 + factor * v2;
}
static inline glm::vec3 lerp(const glm::vec3 &v1, const glm::vec3 &v2, float factor) {
	return (1.0f - factor)*v1 + factor * v2;
}
static inline glm::vec2 lerp(const glm::vec2 &v1, const glm::vec2 &v2, float factor) {
	return (1.0f - factor)*v1 + factor * v2;
}

static float lerp(float f1, float f2, float factor) {
	return (1.0f - factor)*f1 + factor * f2;
}
//两点插值
static VertexOut lerp(const VertexOut & v1, const VertexOut & v2, float factor)
{
	return VertexOut(
	lerp(v1.windowPos, v2.windowPos, factor),
	lerp(v1.worldPos, v2.worldPos, factor),
	lerp(v1.lightPos, v2.lightPos, factor),
	lerp(v1.color, v2.color, factor),
	lerp(v1.texcoord, v2.texcoord, factor),
	lerp(v1.normal, v2.normal, factor),
	v1.TBN * (1.0f - factor) + v2.TBN * factor,
	lerp(v1.Z, v2.Z, factor)
	);
}

//三点插值
static VertexOut lerp(const VertexOut & v1, const VertexOut & v2, const VertexOut & v3, const glm::vec3 & weights)
{
	return VertexOut(
		glm::vec4(
			v1.windowPos.x * weights.x + v2.windowPos.x * weights.y + v3.windowPos.x * weights.z,
			v1.windowPos.y * weights.x + v2.windowPos.y * weights.y + v3.windowPos.y * weights.z,
			v1.windowPos.z * weights.x + v2.windowPos.z * weights.y + v3.windowPos.z * weights.z,
			v1.windowPos.w * weights.x + v2.windowPos.w * weights.y + v3.windowPos.w * weights.z
		),
		glm::vec4(
			v1.worldPos.x * weights.x + v2.worldPos.x * weights.y + v3.worldPos.x * weights.z,
			v1.worldPos.y * weights.x + v2.worldPos.y * weights.y + v3.worldPos.y * weights.z,
			v1.worldPos.z * weights.x + v2.worldPos.z * weights.y + v3.worldPos.z * weights.z,
			1.0f
		),
		glm::vec4(
			v1.lightPos.x * weights.x + v2.lightPos.x * weights.y + v3.lightPos.x * weights.z,
			v1.lightPos.y * weights.x + v2.lightPos.y * weights.y + v3.lightPos.y * weights.z,
			v1.lightPos.z * weights.x + v2.lightPos.z * weights.y + v3.lightPos.z * weights.z,
			v1.lightPos.w * weights.x + v2.lightPos.w * weights.y + v3.lightPos.w * weights.z
		),
		glm::vec4(
			v1.color.x * weights.x + v2.color.x * weights.y + v3.color.x * weights.z,
			v1.color.y * weights.x + v2.color.y * weights.y + v3.color.y * weights.z,
			v1.color.z * weights.x + v2.color.z * weights.y + v3.color.z * weights.z,
			v1.color.w * weights.x + v2.color.w * weights.y + v3.color.w * weights.z
		),
		glm::vec2(
			v1.texcoord.x * weights.x + v2.texcoord.x * weights.y + v3.texcoord.x * weights.z,
			v1.texcoord.y * weights.x + v2.texcoord.y * weights.y + v3.texcoord.y * weights.z
		),
		glm::vec3(
			v1.normal.x * weights.x + v2.normal.x * weights.y + v3.normal.x * weights.z,
			v1.normal.y * weights.x + v2.normal.y * weights.y + v3.normal.y * weights.z,
			v1.normal.z * weights.x + v2.normal.z * weights.y + v3.normal.z * weights.z
		),
		v1.TBN * weights.x + v2.TBN * weights.y + v3.TBN * weights.z,
		glm::dot(glm::vec4(v1.Z, v2.Z, v3.Z, 0.0f), glm::vec4(weights,0.0f))
	);
}

// front (0,0,1)
static glm::vec3 Dir2Euler(const glm::vec3 & fromDir)
{
	glm::vec3 eulerAngles = glm::vec3(1.0f);
	//pitch = arc cos(sqrt((x^2 + z^2)/(x^2+y^2+z^2)))
	eulerAngles.x = glm::degrees(acos(sqrt((fromDir.x * fromDir.x + fromDir.z * fromDir.z) / (fromDir.x * fromDir.x + fromDir.y * fromDir.y + fromDir.z * fromDir.z))));
	if (fromDir.y < 0) 
		eulerAngles.x = -eulerAngles.x;
	//yaw = arc tan(x/z)
	eulerAngles.y = glm::degrees(atan2(fromDir.x, fromDir.z));

	//roll = 0
	eulerAngles.z = 0;
	return eulerAngles;
}

// front (0,0,1)
static glm::vec3 Euler2Dir(const glm::vec3 & fromEuler) {
	
	glm::vec3 dir = glm::vec3(1.0f);
	dir.x = cos(glm::radians(fromEuler.x)) * sin(glm::radians(fromEuler.y));
	dir.y = sin(glm::radians(fromEuler.x));
	dir.z = cos(glm::radians(fromEuler.x)) * cos(glm::radians(fromEuler.y));
	return dir;
}

#endif