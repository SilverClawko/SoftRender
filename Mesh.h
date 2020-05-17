#ifndef MESH_H
#define MESH_H

#include "Global.h"
#include "Vertex.h"


class Material;

class Mesh
{

public:
	std::vector<Vertex> VBO;
	std::vector<unsigned int> EBO;
	Material * material;
	std::string name;


	Mesh();
	Mesh(int  vNum, int  iNum);
	~Mesh();

	Mesh(const Mesh& mesh);
	Mesh& operator=(const Mesh& mesh);
	void AddMesh(const Mesh & mesh);
	void AddTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3);

	static Mesh * CreatePlane(const glm::vec3 &leftTop, const glm::vec3 &leftBottom, const glm::vec3 & rightBottom, const glm::vec3 &rightTop, const glm::vec3 &normal);
	static Mesh * CreateBox(const glm::vec3 & center, float radius);
};

#endif
