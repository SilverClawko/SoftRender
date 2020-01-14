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
		o.windowPos = ProjectMatrix * ViewMatrix * o.worldPos;

		o.normal = glm::normalize(NormalMatrix * a2v.normal);
		o.texcoord = a2v.texcoord;
		o.color = a2v.color;
		return o;
	}
	virtual glm::vec4 FragmentShader(const V2F &v) {
		return v.color;
	}

};

#endif