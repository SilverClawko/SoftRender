#ifndef VERTEX_H
#define VERTEX_H

#include "Global.h"

class Vertex {
public:
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
	glm::vec4 tangent;

	Vertex() = default;
	~Vertex() = default;

	Vertex(
		const glm::vec4 & _pos,
		const glm::vec4 & _color,
		const glm::vec2 & _tex,
		const glm::vec3 & _normal,
		const glm::vec4 & _tangent
	);
	Vertex(
		const glm::vec3 & _pos,
		const glm::vec4 & _color = glm::vec4(0, 0, 0, 0),
		const glm::vec2 & _tex = glm::vec2(0, 0),
		const glm::vec3 & _normal = glm::vec3(0, 1, 0),
		const glm::vec4 & _tangent = glm::vec4(0, 0, 1, 1)
	);

	static glm::vec4 CalcTangent(const Vertex &v1 , const Vertex &v2 , const Vertex &v3);
};

class VertexOut {
public:
	//SV_POSITION
	glm::vec4 windowPos;  
	glm::vec4 worldPos;
	glm::vec4 lightPos;
	glm::vec4 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
	glm::mat3 TBN;
	float Z;

	VertexOut(
		const glm::vec4 &_pos = glm::vec4(1.0f),
		const glm::vec4 &_wpos = glm::vec4(1.0f),
		const glm::vec4 &_lpos = glm::vec4(1.0f),
		const glm::vec4 &_c = glm::vec4(1.0f),
		const glm::vec2 &_t = glm::vec2(1.0f),
		const glm::vec3 &_n = glm::vec3(0.0f,1.0,0.0f),
		const glm::mat3 &_tbn = glm::mat3(1.0f),
		float z = 1.0f
	);

	VertexOut operator+ (const VertexOut &v);
	VertexOut operator- (const VertexOut &v);
	VertexOut operator* (float f);
	VertexOut & operator+=(const VertexOut &v);
	friend VertexOut  operator*(float f, const VertexOut & v);
};

#endif