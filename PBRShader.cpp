#include "PBRShader.h"
#include "Vertex.h"
#include "Function.h"
#include "Texture.h"
#include "Light.h"

PBRShader * PBRShader::shader = nullptr;
Texture2D * PBRShader::brdfLUT = nullptr;

inline float DistributionGGX(const glm::vec3 &N, const glm::vec3 &H, float a)
{
	float a2 = a * a;
	float NdotH = max(glm::dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

inline float GeometrySchlickGGX(float NdotV, float k)
{
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	return nom / denom;
}

inline float GeometrySmith(const glm::vec3 &N, const glm::vec3 &V, const glm::vec3 &L, float k)
{
	float NdotV = max(glm::dot(N, V), 0.0);
	float NdotL = max(glm::dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, k);
	float ggx2 = GeometrySchlickGGX(NdotL, k);
	return ggx1 * ggx2;
}

inline glm::vec3 FresnelSchlick(float cosTheta, const glm::vec3 &F0)
{
	return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}




PBRShader::~PBRShader()
{
}

PBRShader * PBRShader::GetInstance()
{
	if (shader == nullptr) 
		shader = new PBRShader();
	if (brdfLUT == nullptr)
		brdfLUT = new Texture2D("assets/ibl_brdf_lut.png");
	return shader;
}

VertexOut PBRShader::VertexShader(const Vertex & v)
{
	VertexOut o;
	o.worldPos = uniform->modelMatrix * v.position;
	// PVM*v
	o.windowPos = uniform->MVP * v.position;
	o.normal = glm::normalize(uniform->normalMatrix * v.normal);
	o.texcoord = v.texcoord;
	o.lightPos = uniform->dirLight->lightMatrix * o.worldPos;
	glm::vec3 tangent = uniform->normalMatrix * (glm::vec3)v.tangent;
	if (tangent != glm::vec3(0.0f)) {
		tangent = glm::normalize(tangent);
	}
	glm::vec3 binormal = glm::cross(o.normal, tangent);
	if (binormal != glm::vec3(0.0f))
		binormal = glm::normalize(binormal * v.tangent.w);

	o.TBN = glm::mat3(tangent, binormal, o.normal);
	//o.TBN = glm::mat3(1.0f);
	return o;
}

glm::vec4 PBRShader::FragmentShader(const VertexOut & f)
{
	glm::vec3 albedo = uniform->color;
	if (uniform->mainTex)
		albedo = uniform->mainTex->Sample2D(f.texcoord);
	glm::vec3 normal; 
	if (uniform->normalTex) {
		normal = glm::normalize(f.TBN * (uniform->normalTex->Sample2D(f.texcoord) * 2.0f - 1.0f));
	}
	else
		normal = glm::normalize(f.normal);

	glm::vec3 viewDir = glm::normalize(uniform->cameraPos - f.worldPos);
	glm::vec3 reflectDir = glm::normalize(glm::reflect(-viewDir, normal));

	float metallic = uniform->metallic;
	if (uniform->metallicTex)
		metallic = uniform->metallicTex->Sample2D(f.texcoord).r;
	float smoothness = 1.0f - uniform->roughness;
	if(uniform->roughnessTex)
		smoothness = 1.0f - uniform->roughnessTex->Sample2D(f.texcoord).r;
	float ao = 1.0f;
	if (uniform->aoTex)
		ao = uniform->aoTex->Sample2D(f.texcoord).r;

	glm::vec3 f0 = lerp(uniform->rF0, albedo, metallic);

	glm::vec3 color = CalcDirLight(
		uniform->dirLight,
		viewDir,
		normal,
		f.lightPos,
		albedo,
		metallic,
		smoothness,
		f0
	);

	glm::vec3 fresnel = FresnelSchlick(max(glm::dot(viewDir, normal),0.0f), f0);
	glm::vec3 kS = fresnel;
	glm::vec3 kD = 1.0f - kS;
	kD *= 1.0 - metallic;
	glm::vec3 irradiance(0.1f);
	if (uniform->irradiance)
		irradiance = uniform->irradiance->SampleCUBE(normal);

	glm::vec3 diffuse = irradiance * albedo;
	glm::vec3 prefilteredColor(0.1f);
	if (uniform->radiance)
		prefilteredColor = uniform->radiance->SampleCUBE(reflectDir);

	glm::vec2 envBRDF = brdfLUT->Sample2D(glm::vec2(max(glm::dot(normal, viewDir), 0.0), smoothness));
	glm::vec3 specular = prefilteredColor * (fresnel * envBRDF.x + envBRDF.y);

	//float NDF = DistributionGGX(normal,)_
	glm::vec3 ambient = (kD * diffuse + specular) * ao;
	color = color + ambient;
	//color = color / (color + glm::vec3(1.0f));
	//color = glm::pow(color,glm::vec3(0.45f));
	return glm::vec4(color, 1.0f);
}


inline glm::vec4 PBRShader::CalcDirLight(
	DirectionLight * dirLight,
	const glm::vec3 & viewDir,
	const glm::vec3 & normal,
	const glm::vec4 & lightPos,
	const glm::vec3 & albedo,
	float metallic,
	float smoothness,
	const glm::vec3 & rF0
)
{
	glm::vec3 radiance = dirLight->color * dirLight->intensity;
	glm::vec3 lightDir = glm::normalize(-dirLight->direction);
	glm::vec3 halfDir = glm::normalize(viewDir + lightDir);

	//cook-torrance BRDF
	float NDF = DistributionGGX(normal, halfDir, smoothness);
	float G = GeometrySmith(normal, viewDir, lightDir, smoothness);

	glm::vec3 fresnel = FresnelSchlick(max(glm::dot(halfDir, viewDir), 0.0f), rF0);
	glm::vec3 kS = fresnel;
	glm::vec3 kD = glm::vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	glm::vec3 nominator = NDF * G * fresnel;
	float denominator = 4.0 * max(glm::dot(normal, viewDir), 0.0f) * max(glm::dot(normal, lightDir), 0.0f) + 0.001f;
	glm::vec3 specular = nominator / denominator;

	float NdotL = max(glm::dot(normal, lightDir), 0.0);
	glm::vec3 color = (kD * albedo / PI + specular) * radiance * NdotL;

	float bias = max(0.05f * (1.0f - glm::dot(normal, lightDir)), 0.005f);
	//float shadow = CalcShadow(lightPos, bias);

	return  glm::vec4(color, 1.0f);
}

inline float PBRShader::CalcShadow(const glm::vec4 & lightPos, float bias)
{
	if (uniform->dirLight->ShadowMap()) {
		glm::vec3 projPos = lightPos;
		projPos /= lightPos.w;
		projPos = projPos * 0.5f + 0.5f;

		float closestDepth = uniform->dirLight->ShadowMap()->Sample2D(glm::vec2(projPos.x, projPos.y)).r;
		float currentDepth = projPos.z;
		float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
		return shadow;
	}
	else {
		return 0.0f;
	}
}


