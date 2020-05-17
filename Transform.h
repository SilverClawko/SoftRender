#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Global.h"

class Transform {

public:
	static glm::mat4 GetViewPortMatrix(int ox, int oy, int width, int height);

	static glm::mat4 GetViewMatrix(const glm::vec3 &pos, const glm::vec3 & front, const glm::vec3 & right, const glm::vec3 & up);

	static glm::mat4 GetPerspectiveMatrix(float fovy, float aspect, float n, float f);

	static glm::mat4 GetOrthoMatrix(int w, int h, float n, float f);
};


#endif