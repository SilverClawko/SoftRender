
#ifndef BLINNPHONG_H
#define BLINNPHONG_H
#include "Shader.h"
#include "Light.h"


class BlinnPhongShader : public Shader {

public:
	DirectionLight * dirLight;
	PointLight * ptLight;
	SpotLight * spLight;

	BlinnPhongShader() : dirLight(nullptr),ptLight(nullptr),spLight(nullptr)
	{}

	void SetDirectionLight(DirectionLight * l) {
		dirLight = l;
	}
	void SetPointLight(PointLight * p) {
		ptLight = p;
	}
	void SetSpotLight(SpotLight * s) {
		spLight = s;
	}

	virtual glm::vec4 FragmentShader(const V2F &v) {
		if (!currentMat->MainTex) {
			return glm::vec4(1.0);
		}
		glm::vec4 albedo = currentMat->MainTex->Sample2D(v.texcoord) * currentMat->Color;
		glm::vec3 worldNormal = glm::normalize(v.normal);
		glm::vec3 worldViewDir = glm::normalize(camera->Position - glm::vec3(v.worldPos));

		glm::vec3 result = Ambient * glm::vec3(albedo);
		if(dirLight)
			result += dirLight->CalcDirLight(worldNormal, worldViewDir, albedo);
		if (ptLight)
			result += ptLight->CalcPointLight(glm::vec3(v.worldPos),worldNormal,worldViewDir,albedo);
		if (spLight)
			result += spLight->CalcSpotLight(glm::vec3(v.worldPos), worldNormal, worldViewDir, albedo);
		return glm::vec4(result, 1.0);
	}


};





#endif