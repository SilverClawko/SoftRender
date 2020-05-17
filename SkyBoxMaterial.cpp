#include "SkyBoxMaterial.h"
#include "SkyboxShader.h"
#include "Texture.h"

SkyBoxMaterial::SkyBoxMaterial() : _Skybox(nullptr)
{
	shaders[0] = nullptr;
	shaders[1] = SkyBoxShader::GetInstance();
	_Skybox = std::make_shared<Texture3D>("skybox/skybox.hdr");
}

SkyBoxMaterial::SkyBoxMaterial(const SkyBoxMaterial & b) : _Skybox(b._Skybox)
{
	shaders[0] = nullptr;
	shaders[1] = SkyBoxShader::GetInstance();
}

SkyBoxMaterial & SkyBoxMaterial::operator=(const SkyBoxMaterial & b)
{
	if (&b == this)
		return *this;
	_Skybox = b._Skybox;
	color = b.color;
	return *this;
}

SkyBoxMaterial::~SkyBoxMaterial()
{
	_Skybox = nullptr;
}

void SkyBoxMaterial::SetUniform(Uniform & u)
{
	u.color = color;
	u.cubemap = _Skybox.get();
}

void SkyBoxMaterial::SetTexture(Texture3D * t, int num)
{
	_Skybox = std::make_shared<Texture3D>(*t);
}
