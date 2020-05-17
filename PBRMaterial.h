#ifndef PBRMATERIAL_H
#define PBRMATERIAL_H

#include "BlinnMaterial.h"

class Texture3D;


class PBRMaterial : public BlinnMaterial{

protected:
	std::shared_ptr<Texture2D> _Normal;
	std::shared_ptr<Texture2D> _Metallic;
	std::shared_ptr<Texture2D> _Roughness;
	std::shared_ptr<Texture2D> _AO;
	std::shared_ptr<Texture3D> _Irradiance;
	std::shared_ptr<Texture3D> _Radiance;

public:
	glm::vec3 rF0;

	PBRMaterial();
	PBRMaterial(const PBRMaterial &p);
	PBRMaterial(const BlinnMaterial &b);
	PBRMaterial & operator= (const PBRMaterial &p);
	PBRMaterial & operator= (const BlinnMaterial &b);
	virtual ~PBRMaterial();
	virtual void SetUniform(Uniform &u);
	virtual void SetTexture(Texture2D * t, int num);
	virtual void SetTexture(Texture3D * t, int num);
};




#endif