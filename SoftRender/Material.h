#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"

//标准光照材质包括
//漫反射颜色 镜面反射颜色 镜面反射强度
//纹理一张
class Material {
public:
	glm::vec4 Color;
	glm::vec4 Specular;
	int Gloss;   // 8 - 256
	Texture * MainTex;
	Shader * shader;

	Material() :
		Color(glm::vec4(1.0, 1.0, 1.0,1.0)),
		Specular(glm::vec4(1.0, 1.0, 1.0,1.0)),
		Gloss(32),
		MainTex(nullptr)
	{}
	Material(const glm::vec4 &color,const glm::vec4 & specular,const int & gloss):
		Color(color),
		Specular(specular),
		Gloss(gloss),
		MainTex(nullptr)
	{}

	~Material() = default;

	void SetShader(Shader * s) {
		shader = s;
	}
	void SetTexture(Texture * t) {
		MainTex = t;
	}
};













#endif