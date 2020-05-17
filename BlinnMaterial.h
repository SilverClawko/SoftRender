#ifndef BLINNMAT_H
#define BLINNMAT_H

#include "MaterialBase.h"

class Texture2D;

class BlinnMaterial : public Material {

protected:
	std::shared_ptr<Texture2D>  _MainTex;
	
public:
	glm::vec4 specular;
	float gloss;

	BlinnMaterial();
	BlinnMaterial(const BlinnMaterial &b);
	BlinnMaterial & operator= (const BlinnMaterial &b);
	virtual ~BlinnMaterial();
	virtual void SetUniform(Uniform &u);
	virtual void SetTexture(Texture2D * t, int num);

};




#endif