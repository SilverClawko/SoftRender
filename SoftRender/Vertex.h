#ifndef VERTEX_H
#define VERTEX_H

#include "Math.h"

//顶点
class Vertex {
public:
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 texcoord;
	glm::vec3 normal;

	Vertex() = default;
	~Vertex() = default;

	Vertex(
		const glm::vec4 & _pos,
		const glm::vec4 & _color,
		const glm::vec2 & _tex,
		const glm::vec3 & _normal
	) :
		position(_pos), color(_color), texcoord(_tex), normal(_normal) {}
	Vertex(
		const glm::vec3 & _pos,
		const glm::vec4 & _color = glm::vec4(0, 0, 0, 0),
		const glm::vec2 & _tex = glm::vec2(0, 0),
		const glm::vec3 & _normal = glm::vec3(0, 0, 1)
	) :
		position(_pos,1.0f), color(_color), texcoord(_tex), normal(_normal) {}
	Vertex(const Vertex &v) :position(v.position), color(v.color), texcoord(v.texcoord), normal(v.normal) {}
};

class V2F {
public:
	glm::vec4 worldPos;
	glm::vec4 windowPos;
	glm::vec4 color;
	glm::vec2 texcoord;
	glm::vec3 normal;

	float Z;

	V2F() = default;
	~V2F() = default;
	V2F(
		const glm::vec4 & _wPos,
		const glm::vec4 & _pPos,
		const glm::vec4 & _color, 
		const glm::vec2 & _tex,
		const glm::vec3 & _normal
	) :
		worldPos(_wPos), windowPos(_pPos), color(_color), texcoord(_tex), normal(_normal) {}
	V2F(const V2F &v) : 
		worldPos(v.worldPos), windowPos(v.windowPos), color(v.color), texcoord(v.texcoord), normal(v.normal),Z(v.Z) {}

	//两个顶点之间的插值
	static V2F lerp(const V2F &v1, const V2F &v2, const float &factor) {
		V2F result;
		result.windowPos = Lerp(v1.windowPos, v2.windowPos, factor);
		result.worldPos = Lerp(v1.worldPos, v2.worldPos, factor);
		result.color = Lerp(v1.color, v2.color, factor);
		result.normal = Lerp(v1.normal, v2.normal, factor);
		result.texcoord = Lerp(v1.texcoord, v2.texcoord, factor);

		result.Z = Lerp(v1.Z, v2.Z, factor);

		return result;
	}
};

#endif 