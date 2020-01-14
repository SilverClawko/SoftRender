#ifndef CULL_H
#define CULL_H

#include "Globel.h"
#include "Math.h"

enum Face {
	Back,
	Front
};
//世界空间的视锥剔除
bool WorldSpaceCull(const std::vector<glm::vec4> ViewPlanes,const glm::vec4 &v1, const glm::vec4 &v2, const glm::vec4 &v3) {


	if (!Point2Plane(v1, ViewPlanes[4]) && !Point2Plane(v2, ViewPlanes[4]) && !Point2Plane(v3, ViewPlanes[4])) {
		return false;
	}
	//远平面 直接丢弃
	if (!Point2Plane(v1, ViewPlanes[5]) || !Point2Plane(v2, ViewPlanes[5]) || !Point2Plane(v3, ViewPlanes[5])) {
		return false;
	}
	if (!Point2Plane(v1, ViewPlanes[0]) && !Point2Plane(v2, ViewPlanes[0]) && !Point2Plane(v3, ViewPlanes[0])) {
		return false;
	}
	if (!Point2Plane(v1, ViewPlanes[1]) && !Point2Plane(v2, ViewPlanes[1]) && !Point2Plane(v3, ViewPlanes[1])) {
		return false;
	}
	if (!Point2Plane(v1, ViewPlanes[2]) && !Point2Plane(v2, ViewPlanes[2]) && !Point2Plane(v3, ViewPlanes[2])) {
		return false;
	}
	if (!Point2Plane(v1, ViewPlanes[3]) && !Point2Plane(v2, ViewPlanes[3]) && !Point2Plane(v3, ViewPlanes[3])) {
		return false;
	}
	return true;
}

bool ClipSpaceCull(const glm::vec4 &v1, const glm::vec4 &v2, const glm::vec4 &v3) {

	if (v1.w <= camera->Near && v2.w <= camera->Near && v3.w <= camera->Near)
		return false;
	if (v1.w >= camera->Far && v2.w <= camera->Far && v3.w <= camera->Far)
		return false;
	if (v1.x <= v1.w || v1.y <= v1.w || v1.z <= v1.w)
		return true;
	if (v2.x <= v2.w || v2.y <= v2.w || v2.z <= v2.w)
		return true;
	if (v3.x <= v3.w || v3.y <= v3.w || v3.z <= v3.w)
		return true;
	return false;
}

//面剔除，剔除正向面或者逆向面
bool FaceCull(Face face, const glm::vec4 &v1, const glm::vec4 &v2, const glm::vec4 &v3) {

	glm::vec3 tmp1 = glm::vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
	glm::vec3 tmp2 = glm::vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

	//叉乘得到法向量
	glm::vec3 normal = glm::normalize(glm::cross(tmp1, tmp2));
	//glm::vec3 view = glm::normalize(glm::vec3(v1.x - camera->Position.x, v1.y - camera->Position.y, v1.z - camera->Position.z));
	//NDC中观察方向指向+z
	glm::vec3 view = glm::vec3(0, 0, 1);
	if (face == Back)
		return glm::dot(normal, view) > 0;
	else
		return glm::dot(normal, view) < 0;
}



#endif