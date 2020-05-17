#include "MaterialBase.h"
#include "ShadowShader.h"
#include "BlinnShader.h"
#include "Texture.h"

Material::Material() :
	shaders(2, nullptr),
	color(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)),
	specular(glm::vec4(1.0f)),
	gloss(8.0f),
	bump(1.0f),
	metallic(1.0f),
	roughness(0.99f),
	ao(1.0f),
	rF0(glm::vec3(0.04f)),
	mainTex(nullptr),
	normalTex(nullptr),
	metallicTex(nullptr),
	roughnessTex(nullptr),
	aoTex(nullptr)
	//cubemap(nullptr),
	//irradianceMap(nullptr),
	//radianceMap(nullptr)
{
	shaders[0] = ShadowShader::GetInstance();
	shaders[1] = BlinnShader::GetInstance();
}


Material::~Material()
{
}

Shader * Material::GetShader(int pass)
{
	return shaders[pass];
}


void Material::SetShader(Shader * shader, int pass)
{
	shaders[pass] = shader;
}

void Material::SetTexture(Texture2D * t, int num)
{
	switch (num) {
		case 0:
			mainTex = std::make_shared<Texture2D>(*t);
			break;
		case 1:
			normalTex = std::make_shared<Texture2D>(*t);
			break;
		case 2:
			metallicTex = std::make_shared<Texture2D>(*t);
			break;
		case 3:
			roughnessTex = std::make_shared<Texture2D>(*t);
			break;
		case 4:
			aoTex = std::make_shared<Texture2D>(*t);
			break;
	}
}

/*
void Material::SetTexture(Texture3D * t, int num)
{
	switch (num) {
	case 0:
		cubemap = std::make_shared<Texture3D>(*t);
		break;
	case 1:
		irradianceMap = std::make_shared<Texture3D>(*t);
		break;
	case 2:
		radianceMap = std::make_shared<Texture3D>(*t);
		break;
	}
}
*/

void Material::SetupUniform(Uniform & u)
{
	u.color = color;
	u.specular = specular;
	u.gloss = gloss;
	u.bumpScale = bump;
	u.metallic = metallic;
	u.roughness = roughness;
	u.ao = ao;
	u.mainTex = mainTex.get();
	u.normalTex = normalTex.get();
	u.metallicTex = metallicTex.get();
	u.roughnessTex = roughnessTex.get();
	u.aoTex = aoTex.get();
	//u.cubemap = cubemap.get();
	//u.irradiance = irradianceMap.get();
	//u.radiance = radianceMap.get();
}


