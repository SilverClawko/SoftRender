#ifndef BLINN_H
#define BLINN_H

#include "ShaderBase.h"

class DirectionLight;

class BlinnShader : public Shader {

private:
	static BlinnShader * shader;
	BlinnShader() {}

public:


	virtual ~BlinnShader() = default;
	static BlinnShader * GetInstance();

	virtual VertexOut VertexShader(const Vertex &v);
	virtual glm::vec4 FragmentShader(const VertexOut &f);

	inline glm::vec4 CalcDirLight(DirectionLight * dirLight,const glm::vec3 &viewDir,const glm::vec3 &normal,const glm::vec4 &lightPos,const glm::vec4 albedo);
	inline float CalcShadow(const glm::vec4 &lightPos, float bias);
	
	virtual void Destroy() {
		if (shader)
			delete shader;
		shader = nullptr;
		uniform = nullptr;
	}
};



#endif