#ifndef PBR_H
#define PBR_H

#include"BlinnPhongShader.h"


class PBRShader : public Shader {
public:

	virtual V2F VertexShader(const Vertex &a2v) {
		V2F o;
		o.worldPos = ModelMatrix * a2v.position;
		// PVM*v
		o.windowPos = ViewMatrix * o.worldPos;
		o.windowPos = ProjectMatrix * o.windowPos;

		o.Z = 1 / o.windowPos.w;
		o.worldPos *= o.Z;
		o.color = a2v.color * o.Z;
		o.normal = glm::normalize(NormalMatrix * a2v.normal);
		o.texcoord = a2v.texcoord * o.Z;
		glm::vec3 T = glm::normalize(NormalMatrix * a2v.tangent);
		glm::vec3 B = glm::normalize(glm::cross(T, o.normal));
		o.TBN = glm::mat3(T,B,o.normal);
		return o;
	}
	virtual glm::vec4 FragmentShader(const V2F &v) {
		if (!currentMat->MainTex) {
			return glm::vec4(1.0);
		}
		glm::vec4 albedo = currentMat->MainTex->Sample2D(v.texcoord) * currentMat->Color;
		glm::vec3 worldNormal = glm::normalize(v.TBN * glm::normalize(currentMat->Normal->Sample2D(v.texcoord) * 2.0f - 1.0f));

		glm::vec3 worldViewDir = glm::normalize(camera->Position - glm::vec3(v.worldPos));

		glm::vec3 result = Ambient * glm::vec3(albedo);
		for(int i = 0;i < dirLtNums;i++)
			result += BlinnPhongShader::CalcDirLight(*(dirLights+i), worldNormal, worldViewDir, albedo);
		for (int i = 0; i < ptLtNums; i++)
			result += BlinnPhongShader::CalcPtLight(*(ptLights+i), glm::vec3(v.worldPos), worldNormal, worldViewDir, albedo);
		for (int i = 0; i < spLtNums; i++)
			result += BlinnPhongShader::CalcSpLight(*(spLights+i), glm::vec3(v.worldPos), worldNormal, worldViewDir, albedo);
		return glm::vec4(result, 1.0);
	}
};


#endif