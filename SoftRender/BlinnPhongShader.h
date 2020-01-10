
#ifndef BLINNPHONG_H
#define BLINNPHONG_H
#include "Shader.h"
#include "Light.h"


class BlinnPhongShader : public Shader {

public:

	BlinnPhongShader() = default;
	virtual ~BlinnPhongShader() = default;

	//向量都要是单位向量
	//ViewDir是片段指向摄像机的方向
	static glm::vec3 CalcDirLight(
		const DirectionLight &dirLight,
		const glm::vec3 &worldNormal,
		const glm::vec3 &worldViewDir,
		const glm::vec3 &albedo
	) {
		float diff = max(glm::dot(worldNormal, -dirLight.Direction), 0);

		glm::vec3 halfDir = glm::normalize(worldViewDir - dirLight.Direction);
		float spec = pow(max(glm::dot(halfDir, worldNormal), 0), currentMat->Gloss);

		glm::vec3 diffuse = dirLight.Color * diff * albedo;
		glm::vec3 specular = dirLight.Specular * spec;
		return  (diffuse + specular)*dirLight.Intensity;
	}
	static glm::vec3 CalcPtLight(
		const PointLight &ptLight,
		const glm::vec3 &worldPos,
		const glm::vec3 &worldNormal,
		const glm::vec3 &worldViewDir,
		const glm::vec3 &albedo
	) {
		float distance = glm::distance(worldPos, ptLight.Position);
		float attenuation = 1.0 / (ptLight.Constant + ptLight.Linear * distance +
			ptLight.Quadratic * (distance * distance));


		glm::vec3 lightDir = glm::normalize(worldPos - ptLight.Position);
		float diff = max(glm::dot(worldNormal, -lightDir), 0);

		glm::vec3 halfDir = glm::normalize(worldViewDir - lightDir);
		float spec = pow(max(glm::dot(halfDir, worldNormal), 0), currentMat->Gloss);

		glm::vec3 diffuse = ptLight.Color * diff * albedo;
		glm::vec3 specular = ptLight.Specular * spec;

		diffuse *= attenuation;
		specular *= attenuation;

		return  (diffuse + specular)*ptLight.Intensity;
	}
	static glm::vec3 CalcSpLight(
		const SpotLight &spLight,
		const glm::vec3 &worldPos,
		const glm::vec3 &worldNormal,
		const glm::vec3 &worldViewDir,
		const glm::vec3 &albedo
	) {

		glm::vec3 lightDir = glm::normalize(worldPos - spLight.Position);
		float theta = glm::dot(lightDir, glm::normalize(spLight.Direction));
		float weight = saturate((theta - spLight.outterCutOff) / (spLight.innerCutOff - spLight.outterCutOff));
		float intensity = Lerp(0, 1, weight);

		float distance = glm::distance(worldPos, spLight.Position);
		float attenuation = 1.0 / (spLight.Constant + spLight.Linear * distance +
			spLight.Quadratic * (distance * distance));
		float diff = max(glm::dot(worldNormal, -lightDir), 0);

		glm::vec3 halfDir = glm::normalize(worldViewDir - lightDir);
		float spec = pow(max(glm::dot(halfDir, worldNormal), 0), currentMat->Gloss);

		glm::vec3 diffuse = spLight.Color * diff * albedo;
		glm::vec3 specular = spLight.Specular * spec;

		diffuse *= (attenuation * intensity);
		specular *= (attenuation * intensity);

		return  (diffuse + specular)*spLight.Intensity;
	}

	virtual glm::vec4 FragmentShader(const V2F &v) {
		if (!currentMat->MainTex) {
			return glm::vec4(1.0);
		}
		glm::vec4 albedo = currentMat->MainTex->Sample2D(v.texcoord) * currentMat->Color;
		glm::vec3 worldNormal = glm::normalize(v.normal);
		glm::vec3 worldViewDir = glm::normalize(camera->Position - glm::vec3(v.worldPos));

		glm::vec3 result = Ambient * glm::vec3(albedo);
		for (int i = 0; i < dirLtNums; i++)
			result += BlinnPhongShader::CalcDirLight(*(dirLights + i), worldNormal, worldViewDir, albedo);
		for (int i = 0; i < ptLtNums; i++)
			result += BlinnPhongShader::CalcPtLight(*(ptLights + i), glm::vec3(v.worldPos), worldNormal, worldViewDir, albedo);
		for (int i = 0; i < spLtNums; i++)
			result += BlinnPhongShader::CalcSpLight(*(spLights + i), glm::vec3(v.worldPos), worldNormal, worldViewDir, albedo); 
		return glm::vec4(result, 1.0);
	}
};

#endif