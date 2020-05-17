#ifndef PBRSHADER_H
#define PBRSHADER_H

#include "ShaderBase.h"

class Texture2D;

class PBRShader : public Shader {

private:
	static PBRShader * shader;
	static Texture2D * brdfLUT;

	PBRShader() {}
	PBRShader(const PBRShader &s) = delete;

public:
	virtual ~PBRShader();
	static PBRShader * GetInstance();

	virtual VertexOut VertexShader(const Vertex &v);
	virtual glm::vec4 FragmentShader(const VertexOut &f);

	inline glm::vec4 CalcDirLight(
		DirectionLight * dirLight,
		const glm::vec3 & viewDir,
		const glm::vec3 & normal,
		const glm::vec4 & lightPos,
		const glm::vec3 & albedo,
		float metallic,
		float smoothness,
		const glm::vec3 & rF0
	);
	inline float CalcShadow(const glm::vec4 &lightPos, float bias);
	
	virtual void Destroy() {
		uniform = nullptr;
		if (shader)
			delete shader;
		if (brdfLUT)
			delete brdfLUT;
		shader = nullptr;
	}
};

#endif