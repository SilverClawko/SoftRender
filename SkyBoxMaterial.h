#pragma once
#include "MaterialBase.h"


class SkyBoxMaterial : public Material {

private:
	std::shared_ptr<Texture3D> _Skybox;

public:
	SkyBoxMaterial();
	SkyBoxMaterial(const SkyBoxMaterial &b);
	SkyBoxMaterial & operator= (const SkyBoxMaterial &b);
	virtual ~SkyBoxMaterial();
	virtual void SetUniform(Uniform &u);
	virtual void SetTexture(Texture3D * t, int num);
};