#ifndef SKYBOX_H
#define SKYBOX_H

#include "Shader.h"
#include "CubeMap.h"

class SkyBoxShader : public Shader {

private:
	CubeMap *cubeMap;


public:
	void SetCubeMap(CubeMap * t) {
		cubeMap = t;
	}

	SkyBoxShader() = default;
	~SkyBoxShader() = default;

	virtual V2F VertexShader(const Vertex &a2v) {
		V2F o;
		glm::mat4 rotView = glm::mat3(ViewMatrix);
		o.worldPos = a2v.position;
		glm::vec4 clipPos = ProjectMatrix * rotView * o.worldPos;
		o.windowPos = glm::vec4(clipPos.x, clipPos.y, clipPos.w, clipPos.w);
		o.Z = 1;
		return o;
	}

	virtual glm::vec4 FragmentShader(const V2F& v) {
		glm::vec3 uv = glm::normalize(v.worldPos); // make sure to normalize localPos
		glm::vec3 color = Pow(cubeMap->Sample3D(uv),0.45);

		//color = color / (color + glm::vec3(1.0));
		//color = pow(color, glm::vec3(1.0 / 2.2));
		return glm::vec4(color, 1.0);
	}

	static glm::vec2 SampleSphericalMap(const glm::vec3 &v)
	{
		glm::vec2 invAtan = glm::vec2(0.1591, 0.3183);
		glm::vec2 uv = glm::vec2(glm::atan(v.z, v.x), glm::asin(v.y));
		uv *= invAtan;
		uv += 0.5;
		return uv;
	}

};


#endif