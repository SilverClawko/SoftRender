#include "BlinnShader.h"
#include "Vertex.h"
#include "Texture.h"
#include "Light.h"
#include "Function.h"

BlinnShader * BlinnShader::shader = nullptr;

BlinnShader * BlinnShader::GetInstance()
{
	if (shader == nullptr)
		shader = new BlinnShader();
	return shader;
}


VertexOut BlinnShader::VertexShader(const Vertex & v)
{
	VertexOut o;
	o.windowPos = uniform->MVP * v.position;
	o.worldPos = uniform->modelMatrix * v.position;
	o.color = v.color;
	o.texcoord = glm::vec4(v.texcoord, 1.0f, 1.0f);
	o.normal = glm::vec4(uniform->normalMatrix * v.normal, 1.0f);
	o.lightPos = uniform->dirLight->lightMatrix * o.worldPos;
	return o;
}


glm::vec4 BlinnShader::FragmentShader(const VertexOut & f)
{
	glm::vec4 albedo = uniform->color;
	if (uniform->mainTex)
		albedo = uniform->mainTex->Sample2D(f.texcoord);
	glm::vec3 worldNormal = glm::normalize(f.normal);
	glm::vec3 worldViewDir = glm::normalize(uniform->cameraPos - f.worldPos);
	
	glm::vec3 color = glm::vec4(0.0f);
	color = CalcDirLight(uniform->dirLight, worldViewDir, worldNormal, f.lightPos, albedo);
	glm::vec3 ambient = uniform->ambient * albedo;
	color = ambient + color;

	return glm::vec4(color, 1.0f);
	//return glm::vec4(shadow, shadow, shadow, 1.0f);
}

inline glm::vec4 BlinnShader::CalcDirLight(DirectionLight * dirLight, const glm::vec3 & viewDir, const glm::vec3 & normal, const glm::vec4 &lightPos, const glm::vec4 albedo)
{
	glm::vec3 lightDir = -dirLight->direction;
	float diff = glm::dot(normal, lightDir) * 0.5f + 0.5f;
	glm::vec3 diffuse = diff * dirLight->color * albedo;
	glm::vec3 halfDir = glm::normalize(viewDir + lightDir);
	float spec = pow(max(glm::dot(normal, halfDir), 0.0f), uniform->gloss);
	glm::vec3 specular = spec * uniform->specular * dirLight->color;
	float bias = max(0.05f * (1.0f - glm::dot(normal, lightDir)), 0.005f);
	float shadow = CalcShadow(lightPos, bias);

	return (dirLight->intensity * (1.0f - shadow)) * glm::vec4(diffuse + specular, 1.0f);
}

inline float BlinnShader::CalcShadow(const glm::vec4 & lightPos, float bias)
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


