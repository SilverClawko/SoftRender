#include "ShadowShader.h"
#include "Vertex.h"
#include "Light.h"

ShadowShader * ShadowShader::shader = nullptr;

ShadowShader * ShadowShader::GetInstance()
{
	if (shader == nullptr)
		shader = new ShadowShader();
	return shader;
}

glm::vec4 ShadowShader::FragmentShader(const VertexOut & f)
{
	return glm::vec4(1.0f);
}




