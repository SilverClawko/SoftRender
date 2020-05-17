#include "Mesh.h"
#include "Vertex.h"
#include "MaterialBase.h"


Mesh::Mesh() : name("Unnamed") , material(nullptr)
{
	material = new Material();
}

Mesh::Mesh(int vNum, int iNum) : material(nullptr),name("Unnamed") {
	VBO.resize(vNum);
	EBO.resize(iNum);
	material = new Material();
}

Mesh::~Mesh()
{
	if(material)
		delete material;
}


Mesh::Mesh(const Mesh & mesh)
	:VBO(mesh.VBO), EBO(mesh.EBO) ,material(mesh.material)
{
}

Mesh& Mesh::operator=(const Mesh & mesh)
{
	if (&mesh == this)
		return *this;
	VBO = mesh.VBO;
	EBO = mesh.EBO;
	material = mesh.material;
	return *this;
}


void Mesh::AddMesh(const Mesh & mesh)
{
	int offset = VBO.size();
	VBO.insert(VBO.end(), mesh.VBO.begin(), mesh.VBO.end());
	EBO.reserve(EBO.size() + mesh.EBO.size());
	for (int i = 0; i < mesh.EBO.size(); i++) {
		EBO.push_back(mesh.EBO[i] + offset);
	}
}

void Mesh::AddTriangle(const Vertex & v1, const Vertex & v2, const Vertex & v3)
{
	int offset = VBO.size();
	VBO.push_back(v1);
	VBO.push_back(v2);
	VBO.push_back(v3);
	EBO.push_back(offset);
	EBO.push_back(offset + 1);
	EBO.push_back(offset + 2);
}


Mesh * Mesh::CreatePlane(
	const glm::vec3 & leftTop,
	const glm::vec3 & leftBottom,
	const glm::vec3 & rightBottom,
	const glm::vec3 & rightTop,
	const glm::vec3 & normal
)
{
	Mesh * result = new Mesh(4, 6);
	result->VBO[0].position = glm::vec4(leftTop, 1.0f);
	result->VBO[0].normal = normal;
	result->VBO[0].color = glm::vec4(1.0f, 0, 0, 1.0f);
	result->VBO[0].texcoord = glm::vec2(0.0f, 1.0f);
	result->VBO[1].position = glm::vec4(rightTop, 1.0f);
	result->VBO[1].normal = normal;
	result->VBO[1].color = glm::vec4(0, 1.0f, 0, 1.0);
	result->VBO[1].texcoord = glm::vec2(1.0f, 1.0f);
	result->VBO[2].position = glm::vec4(rightBottom, 1.0f);
	result->VBO[2].normal = normal;
	result->VBO[2].color = glm::vec4(0, 0, 1.0f, 0);
	result->VBO[2].texcoord = glm::vec2(1.0f, 0.0f);
	result->VBO[3].position = glm::vec4(leftBottom, 1.0f);
	result->VBO[3].normal = normal;
	result->VBO[3].color = glm::vec4(1.0f, 0, 1.0, 1.0f);
	result->VBO[3].texcoord = glm::vec2(0.0f, 0.0f);

	result->EBO[0] = 0;
	result->EBO[1] = 2;
	result->EBO[2] = 1;
	result->EBO[3] = 0;
	result->EBO[4] = 3;
	result->EBO[5] = 2;
	return result;
}

Mesh * Mesh::CreateBox(const glm::vec3 & center, float radius)
{
	Mesh * result = new Mesh();
	
	Mesh * front = Mesh::CreatePlane(
		center + glm::vec3(-radius, radius, radius),
		center + glm::vec3(-radius, -radius, radius),
		center + glm::vec3(radius, -radius, radius),
		center + glm::vec3(radius, radius, radius),
		glm::vec3(0, 0, 1)
	);
	result->AddMesh(*front);
	
	Mesh * back = Mesh::CreatePlane(
		center + glm::vec3(radius, radius, -radius),
		center + glm::vec3(radius, -radius, -radius),
		center + glm::vec3(-radius, -radius, -radius),
		center + glm::vec3(-radius, radius, -radius),
		glm::vec3(0, 0, -1)
	);
	result->AddMesh(*back);
	
	Mesh * left = Mesh::CreatePlane(
		center + glm::vec3(-radius, radius, -radius),
		center + glm::vec3(-radius, -radius, -radius),
		center + glm::vec3(-radius, -radius, radius),
		center + glm::vec3(-radius, radius, radius),
		glm::vec3(-1, 0, 0)
	);
	result->AddMesh(*left);
	
	Mesh * right = Mesh::CreatePlane(
		center + glm::vec3(radius, radius, radius),
		center + glm::vec3(radius, -radius, radius),
		center + glm::vec3(radius, -radius, -radius),
		center + glm::vec3(radius, radius, -radius),
		glm::vec3(1, 0, 0)
	);
	result->AddMesh(*right);
	Mesh * up = Mesh::CreatePlane(
		center + glm::vec3(-radius, radius, -radius),
		center + glm::vec3(-radius, radius, radius),
		center + glm::vec3(radius, radius, radius),
		center + glm::vec3(radius, radius, -radius),
		glm::vec3(0, 1, 0)
	);
	result->AddMesh(*up);
	Mesh * down = Mesh::CreatePlane(
		center + glm::vec3(-radius, -radius, radius),
		center + glm::vec3(-radius, -radius, -radius),
		center + glm::vec3(radius, -radius, -radius),
		center + glm::vec3(radius, -radius, radius),
		glm::vec3(0, -1, 0)
	);
	result->AddMesh(*down);
	
	return result;
}
