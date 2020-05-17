#ifndef LIGHT_H
#define LIGHT_H

#include "Global.h"

class Texture2D;
class Uniform;
//light
class Light {

private:
	Texture2D * shadowMap;

public:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec4 color;
	float intensity;

	glm::mat4 viewMatrix;
	glm::mat4 projectorMatrix;
	glm::mat4 lightMatrix;

	Light(
		const glm::vec3 &pos = glm::vec3(0, 1, 0),
		const glm::vec3 &dir = glm::vec3(-1.0f, -1.0f, -1.0f),
		const glm::vec4 &c = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		float i = 1.0f
	);
	virtual ~Light();
	void SetShadowMap(Texture2D * t);
	Texture2D * ShadowMap() const;
	virtual void SetPosition(const glm::vec3 &pos);
	virtual void SetDirection(const glm::vec3 &dir);
};

//direction light
class DirectionLight : public Light {
public:
	DirectionLight(
		const glm::vec3 &dir = glm::vec3(-1.0f, -1.0f, -1.0f),
		const glm::vec4 &c = glm::vec4(1.0f, 1.0f, 1.0f,1.0f),
		float i = 1.0f
	);

	virtual void SetDirection(const glm::vec3 &dir);
};

/*
class PointLight : public Light{
public:
	float radius;
};

class SpotLight : public Light {

};
*/
#endif