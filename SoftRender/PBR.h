#ifndef PBR_H
#define PBR_H

#include"BlinnPhongShader.h"
#include "CubeMap.h"

class PBRShader : public Shader {
public:

	CubeMap * IrradianceMap;
	PBRShader() : IrradianceMap(nullptr) {}
	~PBRShader() = default;

	void SetCubeMap(CubeMap * c) {
		IrradianceMap = c;
	}

	virtual V2F VertexShader(const Vertex &a2v) {
		V2F o;
		o.worldPos = ModelMatrix * a2v.position;
		// PVM*v
		o.windowPos = ViewMatrix * o.worldPos;
		o.windowPos = ProjectMatrix * o.windowPos;


		o.normal = glm::normalize(NormalMatrix * a2v.normal);
		glm::vec3 T = glm::normalize(NormalMatrix * a2v.tangent);
		glm::vec3 B = glm::normalize(glm::cross(T, o.normal));
		o.texcoord = a2v.texcoord;
		o.TBN = glm::mat3(T,B,o.normal);
		return o;
	}
	virtual glm::vec4 FragmentShader(const V2F &v) {

		glm::vec3 albedo = Pow(currentMat->SampleAlbedo(v.texcoord),2.2);
		float metallic = currentMat->SampleMetallic(v.texcoord);
		float roughness = currentMat->SampleRoughness(v.texcoord);
		float ao = currentMat->SampleAO(v.texcoord);
		glm::vec3 N = glm::normalize(v.TBN * glm::normalize(currentMat->SampleNormal(v.texcoord) * 2.0f - 1.0f));
		glm::vec3 V = glm::normalize(camera->Position - glm::vec3(v.worldPos));


		glm::vec3 F0 = glm::vec3(0.04);
		F0 = Lerp(F0, albedo, metallic);

		glm::vec3 Lo = glm::vec3(0.0);
		for (int i = 0; i < dirLtNums; i++)
			PBRShader::CalcDirLight(*(dirLights + i), V, N, albedo, metallic, roughness, F0, Lo);

		for (int i = 0; i < ptLtNums; i++)
			PBRShader::CalcPtLight(*(ptLights + i), v.worldPos, V, N, albedo, metallic, roughness, F0, Lo);

		for (int i = 0; i < spLtNums; i++)
			PBRShader::CalcSpLight(*(spLights+i),v.worldPos, V, N, albedo, metallic, roughness, F0, Lo);

		glm::vec3 kS = fresnelSchlick(max(glm::dot(N,V),0.0),F0);
		glm::vec3 kD = 1.0f - kS;
		kD *= 1.0 - metallic;
		glm::vec3 irradiance(1.0);
		if(IrradianceMap)
			irradiance = IrradianceMap->Sample3D(N);

		glm::vec3 diffuse = irradiance * albedo;
		glm::vec3 ambient = (kD * diffuse) * ao;
		glm::vec3 color = ambient + Lo;

		color = color / (color + glm::vec3(1.0));
		color = pow(color, glm::vec3(1.0 / 2.2));

		return glm::vec4(color, 1.0);
	}

	static void CalcDirLight(
		const DirectionLight &dirLight,
		const glm::vec3 &V,
		const glm::vec3 &N,
		const glm::vec3 &albedo,
		const float metallic,
		const float roughness,
		const glm::vec3 &F0,
		glm::vec3 &Lo
	) {
		glm::vec3 radiance = dirLight.Color * dirLight.Intensity;
		glm::vec3 L = glm::normalize(-dirLight.Direction);
		glm::vec3 H = glm::normalize(V + L);

		//cook-torrance BRDF
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		glm::vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		glm::vec3 kS = F;
		glm::vec3 kD = glm::vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		glm::vec3 nominator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		glm::vec3 specular = nominator / denominator;

		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	static void CalcPtLight(
		const PointLight &ptLight,
		const glm::vec3 &pos,
		const glm::vec3 &V,
		const glm::vec3 &N,
		const glm::vec3 &albedo,
		const float metallic,
		const float roughness,
		const glm::vec3 &F0,
		glm::vec3 &Lo
	) {
		glm::vec3 L = glm::normalize(pos - ptLight.Position);
		float theta = glm::dot(L, glm::normalize(ptLight.Direction));

		float distance = glm::distance(pos, ptLight.Position);
		float attenuation = 1.0 / (distance * distance);
		glm::vec3 radiance = ptLight.Color * attenuation * ptLight.Intensity;

		L = -L;
		glm::vec3 H = glm::normalize(V + L);

		//cook-torrance BRDF
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		glm::vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		glm::vec3 kS = F;
		glm::vec3 kD = glm::vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		glm::vec3 nominator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		glm::vec3 specular = nominator / denominator;

		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}



	static void CalcSpLight(
		const SpotLight &spLight,
		const glm::vec3 &pos,
		const glm::vec3 &V,
		const glm::vec3 &N,
		const glm::vec3 &albedo,
		const float metallic,
		const float roughness,
		const glm::vec3 &F0,
		glm::vec3 &Lo
	) {
		glm::vec3 L = glm::normalize(pos - spLight.Position);
		float theta = glm::dot(L, glm::normalize(spLight.Direction));
		float weight = saturate((theta - spLight.outterCutOff) / (spLight.innerCutOff - spLight.outterCutOff));
		float intensity = Lerp(0, 1, weight);

		float distance = glm::distance(pos, spLight.Position);
		float attenuation = 1.0 / (distance * distance);
		glm::vec3 radiance = spLight.Color * attenuation * intensity * spLight.Intensity;

		L = -L;
		glm::vec3 H = glm::normalize(V + L);

		//cook-torrance BRDF
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		glm::vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		glm::vec3 kS = F;
		glm::vec3 kD = glm::vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		glm::vec3 nominator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
		glm::vec3 specular = nominator / denominator;

		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	static float DistributionGGX(const glm::vec3 &N,const glm::vec3 &H, float a)
	{
		float a2 = a * a;
		float NdotH = max(glm::dot(N, H), 0.0);
		float NdotH2 = NdotH * NdotH;

		float nom = a2;
		float denom = (NdotH2 * (a2 - 1.0) + 1.0);
		denom = PI * denom * denom;

		return nom / denom;
	}

	static float GeometrySchlickGGX(float NdotV, float k)
	{
		float nom = NdotV;
		float denom = NdotV * (1.0 - k) + k;
		return nom / denom;
	}

	static float GeometrySmith(const glm::vec3 &N,const glm::vec3 &V,const glm::vec3 &L, float k)
	{
		float NdotV = max(glm::dot(N, V), 0.0);
		float NdotL = max(glm::dot(N, L), 0.0);
		float ggx1 = GeometrySchlickGGX(NdotV, k);
		float ggx2 = GeometrySchlickGGX(NdotL, k);

		return ggx1 * ggx2;
	}

	static glm::vec3 fresnelSchlick(float cosTheta, const glm::vec3 &F0)
	{
		return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
	}
};


#endif