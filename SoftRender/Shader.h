#ifndef SHADER_H
#define SHADER_H

#include "Texture.h"

class Shader {
public:

	Shader() = default;

	virtual ~Shader() = default;
	virtual V2F VertexShader(const Vertex &a2v) {
		V2F o;
		o.worldPos = ModelMatrix * a2v.position;
		// PVM*v
		o.windowPos = ProjectMatrix * ViewMatrix * o.worldPos;

		o.Z = 1 / o.windowPos.w;
		o.worldPos *= o.Z;
		o.color = a2v.color * o.Z;
		o.normal = NormalMatrix * a2v.normal * o.Z;
		o.texcoord = a2v.texcoord * o.Z;
		return o;
	}
	virtual glm::vec4 FragmentShader(const V2F &v) {
		return v.color;
	}

};

#endif