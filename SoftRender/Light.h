#ifndef LIGHT_H
#define LIGHT_H

#include "Globel.h"


class Light {
public:
	glm::vec3 Position;

	glm::vec3 Color;
	glm::vec3 Specular;
	glm::vec3 Direction;
	float Intensity;

	virtual ~Light() {
	}
	Light(
		const glm::vec3 &pos = glm::vec3(0, 0, 0),
		const glm::vec3 &color = glm::vec3(1, 1, 1),
		const glm::vec3 &specular = glm::vec3(1, 1, 1),
		const glm::vec3 &dir = glm::vec3(0, -1, 0),
		const float & i = 1.0f) :
		Position(pos),
		Color(color),
		Specular(specular),
		Direction(dir),
		Intensity(i)
	{}
};
//定向光 位置不需要
class DirectionLight : public Light {

public:
	DirectionLight(
		const glm::vec3 &dir = glm::normalize(glm::vec3(0, -1, 1)),
		const glm::vec3 &color = glm::vec3(1, 1, 1),
		const glm::vec3 &specular = glm::vec3(1, 1, 1),
		const float & i = 0.1f)
	{
		Position = glm::vec3(0, 0, 0);
		Color = color;
		Specular = specular;
		Direction = dir;
		Intensity = i;
	}
	//向量都要是单位向量
	//ViewDir是片段指向摄像机的方向
	glm::vec3 CalcDirLight(
		const glm::vec3 &worldNormal,
		const glm::vec3 &worldViewDir,
		const glm::vec3 &albedo
	) {
		float diff = max(glm::dot(worldNormal, -Direction), 0);

		glm::vec3 halfDir = glm::normalize(worldViewDir - Direction);
		float spec = pow(max(glm::dot(halfDir, worldNormal), 0), currentMat->Gloss);

		glm::vec3 diffuse = Color * diff * albedo;
		glm::vec3 specular = Specular * spec;
		return  (diffuse + specular)*Intensity;
	}
};
//点光源 方向不需要
class PointLight : public Light {

public:
	float Constant;
	float Linear;
	float Quadratic;

	PointLight(
		const glm::vec3 &pos = glm::vec3(0, 0, 0),
		const glm::vec3 &color = glm::vec3(1, 1, 1),
		const glm::vec3 &specular = glm::vec3(1, 1, 1),
		const float & i = 1.0f,
		const float & c = 1.0f,
		const float & l = 0.09f,
		const float & q = 0.032f
	) : Constant(c), Linear(l), Quadratic(q)
	{
		Position = pos;
		Color = color;
		Specular = specular;
		Direction = glm::vec3(0, 0, 0);
		Intensity = i;
	}

	glm::vec3 CalcPointLight(
		const glm::vec3 &worldPos,
		const glm::vec3 &worldNormal,
		const glm::vec3 &worldViewDir,
		const glm::vec3 &albedo
	) {
		float distance = glm::distance(worldPos, Position);
		float attenuation = 1.0 / (Constant + Linear * distance +
			Quadratic * (distance * distance));


		glm::vec3 lightDir = glm::normalize(worldPos - Position);
		float diff = max(glm::dot(worldNormal, -lightDir), 0);

		glm::vec3 halfDir = glm::normalize(worldViewDir - lightDir);
		float spec = pow(max(glm::dot(halfDir, worldNormal), 0), currentMat->Gloss);

		glm::vec3 diffuse = Color * diff * albedo;
		glm::vec3 specular = Specular * spec;

		diffuse *= attenuation;
		specular *= attenuation;

		return  (diffuse + specular)*Intensity;
	}


};
//探照灯
class SpotLight : public PointLight {

public:
	float innerCutOff;
	float outterCutOff;

	SpotLight(
		const glm::vec3 &pos = glm::vec3(0, 0, 0),
		const glm::vec3 &dir = glm::vec3(0, 0, -1),
		const glm::vec3 &color = glm::vec3(1, 1, 1),
		const glm::vec3 &specular = glm::vec3(1, 1, 1),
		const float & i = 1.0f,
		const float & c = 1.0f,
		const float & l = 0.09f,
		const float & q = 0.032f,
		const float & icut = glm::cos(glm::radians(12.5f)),
		const float & ocut = glm::cos(glm::radians(17.5))
	) : innerCutOff(icut), outterCutOff(ocut)
	{
		Position = pos;
		Color = color;
		Specular = specular;
		Direction = dir;
		Intensity = i;
		Constant = c;
		Linear = l;
		Quadratic = q;
	}

	glm::vec3 CalcSpotLight(
		const glm::vec3 &worldPos,
		const glm::vec3 &worldNormal,
		const glm::vec3 &worldViewDir,
		const glm::vec3 &albedo
	) {

		glm::vec3 lightDir = glm::normalize(worldPos - Position);
		float theta = glm::dot(lightDir, glm::normalize(Direction));
		float weight = saturate((theta - outterCutOff) / (innerCutOff - outterCutOff));
		float intensity = Lerp(0, 1, weight);

		float distance = glm::distance(worldPos, Position);
		float attenuation = 1.0 / (Constant + Linear * distance +
			Quadratic * (distance * distance));
		float diff = max(glm::dot(worldNormal, -lightDir), 0);

		glm::vec3 halfDir = glm::normalize(worldViewDir - lightDir);
		float spec = pow(max(glm::dot(halfDir, worldNormal), 0), currentMat->Gloss);

		glm::vec3 diffuse = Color * diff * albedo;
		glm::vec3 specular = Specular * spec;

		diffuse *= (attenuation * intensity);
		specular *= (attenuation * intensity);

		return  (diffuse + specular)*Intensity;
	}
};


#endif