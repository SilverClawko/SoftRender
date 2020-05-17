#include "BlinnMaterial.h"
#include "BlinnShader.h"
#include "ShadowShader.h"
#include "Texture.h"

BlinnMaterial::BlinnMaterial() : _MainTex(nullptr), specular(glm::vec4(1.0f)), gloss(8.0f)
{
	shaders[0] = ShadowShader::GetInstance();
	shaders[1] = BlinnShader::GetInstance();
}

BlinnMaterial::BlinnMaterial(const BlinnMaterial & b) : _MainTex(b._MainTex),specular(b.specular),gloss(b.gloss)
{
	shaders[0] = ShadowShader::GetInstance();
	shaders[1] = BlinnShader::GetInstance();
}

BlinnMaterial & BlinnMaterial::operator=(const BlinnMaterial & b)
{
	if (&b == this)
		return *this;
	_MainTex = b._MainTex;
	color = b.color;
	specular = b.specular;
	gloss = b.gloss;
	return *this;
}


BlinnMaterial::~BlinnMaterial()
{
	_MainTex = nullptr;
}

void BlinnMaterial::SetUniform(Uniform & u)
{
	u.color = color;
	u.specular = specular;
	u.gloss = gloss;
	u.mainTex = _MainTex.get();
}

void BlinnMaterial::SetTexture(Texture2D * t, int num)
{
	_MainTex = std::make_shared<Texture2D>(*t);
}


