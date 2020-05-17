#ifndef SHADER_H
#define SHADER_H

#include "Global.h"

class Vertex;
class VertexOut;
class StateMachine;
class Texture2D;
class Texture3D;
class DirectionLight;
class PointLight;
class SpotLight;

class Uniform {
public:

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectorMatrix;
	glm::mat3 normalMatrix;
	glm::mat4 MVP;
	glm::vec4 cameraPos;
	
	glm::vec4 color;
	glm::vec4 specular;
	glm::vec4 ambient;
	float gloss;
	float bumpScale;
	float metallic;
	float roughness;
	float ao;
	glm::vec3 rF0;
	DirectionLight * dirLight;
	PointLight * ptLight;
	SpotLight * spLight;

	Texture2D * mainTex;
	Texture2D * normalTex;
	Texture2D * metallicTex;
	Texture2D * roughnessTex;
	Texture2D * aoTex;

	Texture3D * cubemap;
	Texture3D * irradiance;
	Texture3D * radiance;
	
	Uniform();
	Uniform(
		const glm::mat4 &m,
		const glm::mat4 &v,
		const glm::mat4 &p
		);
	~Uniform();
};

class Shader {

private:
	static Shader * shader;
	

protected:
	Uniform * uniform;
	Shader():uniform(nullptr){}
	Shader(const Shader &s) = delete;

public:
	virtual ~Shader() {}
	static Shader * GetInstance();
	void SetUniform(Uniform * u) { uniform = u; }

	virtual void Destroy() {
		uniform = nullptr;
		if (shader)
			delete shader;
		shader = nullptr;
	}
	virtual VertexOut VertexShader(const Vertex &v);
	virtual glm::vec4 FragmentShader(const VertexOut &f);

};
#endif