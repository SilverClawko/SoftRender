#ifndef SHADOW_H
#define SHADOW_H

#include "ShaderBase.h"

class ShadowShader : public Shader{

private:
	static ShadowShader * shader;
	ShadowShader() {}

public:
	static ShadowShader * GetInstance();
	virtual ~ShadowShader() = default;
	virtual glm::vec4 FragmentShader(const VertexOut &f);
	virtual void Destroy() {
		if (shader)
			delete shader;
		shader = nullptr;
		uniform = nullptr;
	}

};



#endif