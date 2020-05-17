#ifndef MATERIAL_H
#define MATERIAL_H

#include "Global.h"

class Shader;
class Texture2D;
class Texture3D;
class Uniform;




class Material {

protected:
	std::vector<Shader *> shaders;

public:
	glm::vec4 color;
	glm::vec4 specular;
	float gloss;
	float bump;
	float metallic;
	float roughness;
	float ao;
	glm::vec3 rF0;

	std::shared_ptr<Texture2D> mainTex;
	std::shared_ptr<Texture2D> normalTex;
	std::shared_ptr<Texture2D> metallicTex;
	std::shared_ptr<Texture2D> roughnessTex;
	std::shared_ptr<Texture2D> aoTex;
	//std::shared_ptr<Texture3D> cubemap;
	//std::shared_ptr<Texture3D> irradianceMap;
	//std::shared_ptr<Texture3D> radianceMap;

	Material();
	virtual ~Material();

	Shader * GetShader(int pass);
	void SetShader(Shader * shader, int pass);
	void SetTexture(Texture2D * t, int num);
	//void SetTexture(Texture3D * t, int num);
	void SetupUniform(Uniform &u);

};



#endif