#include "ShaderBase.h"
#include "Vertex.h"

Shader * Shader::shader = nullptr;

Shader * Shader::GetInstance()
{
	if (shader == nullptr)
		shader = new Shader();
	return shader;
}

VertexOut Shader::VertexShader(const Vertex & v)
{
	VertexOut o;
	o.windowPos = uniform->MVP * v.position;
	o.worldPos = uniform->modelMatrix * v.position;
	o.color = v.color;
	o.texcoord = glm::vec4(v.texcoord,1.0f,1.0f);
	o.normal = glm::vec4(uniform->normalMatrix * v.normal,1.0f);
	return o;
}


glm::vec4 Shader::FragmentShader(const VertexOut & f)
{
	return uniform->color;
}



Uniform::Uniform() :
	modelMatrix(1.0f),
	viewMatrix(1.0f),
	projectorMatrix(1.0f),
	normalMatrix(1.0f),
	MVP(1.0f),
	cameraPos(0.0f, 0.0f, 1.0f, 1.0f),
	ambient(1.0f),
	
	color(0.5f, 0.5f, 0.5f, 1.0f),
	specular(1.0f),
	gloss(8.0),
	bumpScale(1.0f),
	metallic(1.0f),
	roughness(0.99f),
	ao(1.0f),
	rF0(0.04f),

	dirLight(nullptr),
	ptLight(nullptr),
	spLight(nullptr),

	mainTex(nullptr),
	normalTex(nullptr),
	metallicTex(nullptr),
	roughnessTex(nullptr),
	aoTex(nullptr),
	cubemap(nullptr),
	irradiance(nullptr),
	radiance(nullptr)
{
}

Uniform::Uniform(
	const glm::mat4 & m,
	const glm::mat4 & v,
	const glm::mat4 & p
):
	modelMatrix(m),
	viewMatrix(v),
	projectorMatrix(p),
	normalMatrix(m),
	MVP(p*v*m),
	cameraPos(0.0f, 0.0f, 1.0f,1.0f),
	
	color(0.5f, 0.5f, 0.5f, 1.0f),
	specular(1.0f),
	ambient(1.0f),
	gloss(8.0),
	bumpScale(1.0f),
	metallic(1.0f),
	roughness(0.99f),
	ao(1.0f),
	rF0(0.04f),
	
	dirLight(nullptr),
	ptLight(nullptr),
	spLight(nullptr),
	
	mainTex(nullptr),
	normalTex(nullptr),
	metallicTex(nullptr),
	roughnessTex(nullptr),
	aoTex(nullptr),
	cubemap(nullptr),
	irradiance(nullptr),
	radiance(nullptr)
	
{
}

Uniform::~Uniform()
{
	/*
	mainTex = nullptr;
	normalTex = nullptr;
	metallicTex = nullptr;
	roughnessTex = nullptr;
	aoTex = nullptr;
	cubemap = nullptr;
	irradiance = nullptr;
	radiance = nullptr;
	*/
}
