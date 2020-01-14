#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"

//标准光照材质包括
//漫反射颜色 镜面反射颜色 镜面反射强度
//纹理一张
class Material {
private:
	Texture * Albedo;
	Texture *Metallic;
	Texture *Normal;
	Texture *Roughness;
	Texture *AO;

public:
	Shader * shader;
	glm::vec3 Color;
	glm::vec3 Specular;
	int Gloss;   // 8 - 256

	float metallic;
	float roughness;
	float ao;


	Material() :
		Color(glm::vec3(1.0)),
		Specular(glm::vec3(1.0)),
		Gloss(32),
		Albedo(nullptr),
		Metallic(nullptr),
		Normal(nullptr),
		Roughness(nullptr),
		AO(nullptr),
		metallic(1.0),
		roughness(0.1),
		ao(1.0)
	{}
	Material(const glm::vec3 &color,const glm::vec3 & specular,const int & gloss):
		Color(color),
		Specular(specular),
		Gloss(gloss),
		Albedo(nullptr),
		Metallic(nullptr),
		Normal(nullptr),
		Roughness(nullptr),
		AO(nullptr),
		metallic(1.0),
		roughness(0.1),
		ao(1.0)
	{}

	virtual ~Material() = default;

	glm::vec3 SampleAlbedo(const glm::vec2 &texcoord) {
		if (Albedo) {
			return Albedo->Sample2D(texcoord);
		}
		else {
			return Color;
		}
	}
	glm::vec3 SampleNormal(const glm::vec2 &texcoord) {
		if (Normal) {
			return Normal->Sample2D(texcoord);
		}
		else {
			return glm::vec3(0, 0, 1);
		}
	}
	float SampleMetallic(const glm::vec2 &texcoord) {
		if (Metallic) {
			return Metallic->Sample2D(texcoord).r;
		}
		else {
			return metallic;
		}
	}
	float SampleRoughness(const glm::vec2 &texcoord) {
		if (Roughness) {
			return Roughness->Sample2D(texcoord).r;
		}
		else {
			return roughness;
		}
	}
	float SampleAO(const glm::vec2 &texcoord) {
		if (AO) {
			return AO->Sample2D(texcoord).r;
		}
		else {
			return ao;
		}
	}


	void SetShader(Shader * s) {
		shader = s;
	}
	void SetAlbedo(Texture * a) {
		Albedo = a;
	}
	void SetMetallicMap(Texture * m) {
		Metallic = m;
	}
	void SetNormalMap(Texture * n) {
		Normal = n;
	}
	void SetRoughnessMap(Texture * r) {
		Roughness = r;
	}
	void SetAOMap(Texture * ao) {
		AO = ao;
	}
};













#endif