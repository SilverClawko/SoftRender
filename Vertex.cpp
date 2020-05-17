
#include "Vertex.h"
#include "Function.h"



VertexOut::VertexOut(
	const glm::vec4 & _pos,
	const glm::vec4 & _wpos,
	const glm::vec4 & _lpos,
	const glm::vec4 & _c,
	const glm::vec2 & _t,
	const glm::vec3 & _n,
	const glm::mat3 & _tbn,
	float z
) :
	windowPos(_pos),
	worldPos(_wpos),
	lightPos(_lpos),
	color(_c),
	texcoord(_t),
	normal(_n),
	TBN(_tbn),
	Z(z)
{}


VertexOut VertexOut::operator+(const VertexOut & v)
{
	return VertexOut(
		windowPos + v.windowPos,
		worldPos + v.worldPos,
		lightPos + v.lightPos,
		color + v.color,
		texcoord + v.texcoord,
		normal + v.normal,
		TBN + v.TBN,
		Z + v.Z
	);
}

VertexOut VertexOut::operator-(const VertexOut & v)
{
	return VertexOut(
		windowPos - v.windowPos,
		worldPos - v.worldPos,
		lightPos - v.lightPos,
		color - v.color,
		texcoord - v.texcoord,
		normal - v.normal,
		TBN - v.TBN,
		Z - v.Z
	);
}

VertexOut VertexOut::operator*(float f)
{
	return VertexOut(
		windowPos * f,
		worldPos * f,
		lightPos * f,
		color * f,
		texcoord * f,
		normal * f,
		TBN * f,
		Z * f
	);
}
VertexOut & VertexOut::operator+=(const VertexOut & v)
{
	windowPos += v.windowPos;
	worldPos += v.worldPos;
	normal += v.normal;
	texcoord += v.texcoord;
	color += v.color;
	lightPos += v.lightPos;
	TBN += v.TBN;
	Z += v.Z;
	return *this;
}
VertexOut operator*(float f,const VertexOut & v) {
	return VertexOut(
		v.windowPos * f,
		v.worldPos * f,
		v.lightPos * f,
		v.color * f,
		v.texcoord * f,
		v.normal * f,
		v.TBN * f,
		v.Z * f
	);
}


Vertex::Vertex(const glm::vec4 & _pos, const glm::vec4 & _color, const glm::vec2 & _tex, const glm::vec3 & _normal, const glm::vec4 & _tangent) :
	position(_pos), color(_color), texcoord(_tex), normal(_normal), tangent(_tangent) {}

Vertex::Vertex(const glm::vec3 & _pos, const glm::vec4 & _color, const glm::vec2 & _tex, const glm::vec3 & _normal, const glm::vec4 & _tangent) :
	position(_pos, 1.0f), color(_color), texcoord(_tex), normal(_normal), tangent(_tangent) {}

