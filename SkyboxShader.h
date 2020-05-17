#ifndef SKYBOXSHADER_H
#define SKYBOXSHADER_H

#include "ShaderBase.h"

class SkyBoxShader : public Shader {

private:
	static SkyBoxShader * shader;
	SkyBoxShader() {}

	enum vertexLayout {
		WORLDPOS
	};
	enum cubemapLayout {
		SKYBOX
	};


public:
	static SkyBoxShader * GetInstance();
	virtual ~SkyBoxShader() = default;
	virtual VertexOut VertexShader(const Vertex & v);
	virtual glm::vec4 FragmentShader(const VertexOut &f);
	virtual void Destroy() {
		if (shader)
			delete shader;
		shader = nullptr;
		uniform = nullptr;
	}

};




#endif
