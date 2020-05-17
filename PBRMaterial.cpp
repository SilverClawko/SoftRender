#include "ShadowShader.h"
#include "PBRShader.h"
#include "PBRMaterial.h"
#include "Texture.h"

PBRMaterial::PBRMaterial() : _Normal(nullptr),_Metallic(nullptr),_Roughness(nullptr),_AO(nullptr), _Irradiance(nullptr),_Radiance(nullptr), rF0(0.04f)
{
	shaders[0] = ShadowShader::GetInstance();
	shaders[1] = PBRShader::GetInstance();
}

PBRMaterial::PBRMaterial(const PBRMaterial & p) : BlinnMaterial(p), _Normal(p._Normal), _Metallic(p._Metallic), _Roughness(p._Roughness), _AO(p._AO), _Irradiance(p._Irradiance), _Radiance(p._Radiance), rF0(0.04f)
{
	shaders[0] = ShadowShader::GetInstance();
	shaders[1] = PBRShader::GetInstance();
}

PBRMaterial::PBRMaterial(const BlinnMaterial & b) : BlinnMaterial(b), _Normal(nullptr), _Metallic(nullptr), _Roughness(nullptr), _AO(nullptr), _Irradiance(nullptr), _Radiance(nullptr), rF0(0.04f)
{
	shaders[0] = ShadowShader::GetInstance();
	shaders[1] = PBRShader::GetInstance();
}

PBRMaterial & PBRMaterial::operator=(const PBRMaterial &p)
{
	if (&p == this)
		return *this;
	BlinnMaterial::operator=(p);
	rF0 = p.rF0;
	_Normal = p._Normal;
	_Metallic = p._Metallic;
	_Roughness = p._Roughness;
	_AO = p._AO;
	_Irradiance = p._Irradiance;
	_Radiance = p._Radiance;
	return *this;
}

PBRMaterial & PBRMaterial::operator=(const BlinnMaterial & b)
{
	BlinnMaterial::operator=(b);
	return *this;
}

PBRMaterial::~PBRMaterial()
{
	_Normal = nullptr;
	_Metallic = nullptr;
	_Roughness = nullptr;
	_AO = nullptr;
	_Irradiance = nullptr;
	_Radiance = nullptr;
}

void PBRMaterial::SetUniform(Uniform & u)
{
	BlinnMaterial::SetUniform(u);
	u.rF0 = rF0;
	u.normalTex = _Normal.get();
	u.metallicTex = _Metallic.get();
	u.roughnessTex = _Roughness.get();
	u.aoTex = _AO.get();
	u.irradiance = _Irradiance.get();
	u.radiance = _Radiance.get();
}

void PBRMaterial::SetTexture(Texture2D * t, int num)
{
	switch (num) {
		case 0:
			_MainTex = std::make_shared<Texture2D>(*t);
			break;
		case 1:
			_Normal = std::make_shared<Texture2D>(*t);
			break;
		case 2:
			_Metallic = std::make_shared<Texture2D>(*t);
			break;
		case 3:
			_Roughness = std::make_shared<Texture2D>(*t);
			break;
		case 4:
			_AO = std::make_shared<Texture2D>(*t);
			break;
	}
}

void PBRMaterial::SetTexture(Texture3D * t, int num)
{
	if (num == 0)
		_Irradiance = std::make_shared<Texture3D>(*t);
	else
		_Radiance = std::make_shared<Texture3D>(*t);
}
