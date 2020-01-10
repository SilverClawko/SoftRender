#ifndef SHADER_H
#define SHADER_H

#include "Texture.h"
#include "Vertex.h"

class Shader {
public:

	Shader() = default;

	virtual ~Shader() = default;
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
		return o;
	}
	virtual glm::vec4 FragmentShader(const V2F &v) {
		return v.color;
	}

};

#endif