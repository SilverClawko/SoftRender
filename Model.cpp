#include "Model.h"
#include "MaterialBase.h"
#include "SkyboxShader.h"
#include "Texture.h"
#include <fstream>
#include <sstream>

Model::Model(Mesh * m) : 
	position(glm::vec4(0, 0, 0, 1)),
	pitch(0),
	yaw(0),
	roll(0),
	scale(1.0f),
	name("Unnamed")
{
	AddMesh(m);
}

Model::Model(const std::string &filename) :
	position(glm::vec4(0,0,0,1)),
	pitch(0),
	yaw(0),
	roll(0),
	scale(1.0f),
	name("Unnamed")
{
	LoadObj(filename);
}

Model::~Model()
{
	for (int i = 0; i < meshes.size(); i++) {
		if (meshes[i])
			delete meshes[i];
	}
}

glm::mat4 Model::GetModelMatrix() const
{
	//缩放
	glm::mat4 mat(scale);
	mat[3][3] = 1.0f;

	//旋转
	mat = glm::rotate(mat, glm::radians(pitch), glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, glm::radians(yaw), glm::vec3(0, 1, 0));
	mat = glm::rotate(mat, glm::radians(roll), glm::vec3(0, 0, 1));

	//平移
	mat[3][0] = position.x;
	mat[3][1] = position.y;
	mat[3][2] = position.z;


	return mat;
}

void Model::LoadObj(const std::string &filename)
{
	std::ifstream in(filename);
	if (!in) {
		std::cout << "Open Obj File Error !" << std::endl;
		return;
	}
	int start = filename.find_last_of("/") + 1;
	int end = filename.find_last_of(".");
	name = filename.substr(start, end - start);

	std::vector<glm::vec3> vertexs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec4> tangents;
	std::vector<glm::mat3> faces;
	std::string line;

	bool flag = false;
	while (!in.eof()) {
		std::getline(in, line);
		if (!line.compare(0, 2, "v "))
		{
			if (flag) {
				flag = false;
				BuildMesh(vertexs,normals,texcoords,tangents,faces);
				faces.clear();
			}
			line = line.substr(2);
			std::istringstream iss(line);
			glm::vec3 v;
			iss >> v.x;
			iss >> v.y;
			iss >> v.z;
			vertexs.push_back(v);
			continue;
		}
		if (!line.compare(0, 3, "vn "))
		{
			line = line.substr(3);
			std::istringstream iss(line);
			glm::vec3 vn;
			iss >> vn.x;
			iss >> vn.y;
			iss >> vn.z;
			normals.push_back(vn);
			continue;
		}
		if (!line.compare(0, 3, "vt "))
		{
			line = line.substr(3);
			std::istringstream iss(line);
			glm::vec3 vt;
			iss >> vt.x;
			iss >> vt.y;
			vt.y = 1 - vt.y;
			//二维纹理 z=0
			iss >> vt.z;
			texcoords.push_back(glm::vec2(vt.x, vt.y));
			continue;
		}
		if (!line.compare(0, 2, "f "))
		{
			if (!flag) 
				flag = true;
			line = line.substr(2);
			std::istringstream iss(line);
			char bar;
			glm::vec3 vert[4];
			// 1/1/1
			for (int i = 0; i < 3; i++) {
				iss >> vert[i].x >> bar >> vert[i].y >> bar >> vert[i].z;
			}
			glm::mat3 face = glm::mat3(vert[0], vert[1], vert[2]);
			faces.push_back(face);
			if (iss >> vert[3].x) {
				iss >> bar >> vert[3].y >> bar >> vert[3].z;
				face = glm::mat3(vert[0],vert[2],vert[3]);
				faces.push_back(face);
			}
			continue;
		}
		if (!line.compare(0, 14,"# ext.tangent ")) {
			line = line.substr(14);
			std::istringstream iss(line);
			glm::vec4 tangent;
			iss >> tangent.x;
			iss >> tangent.y;
			iss >> tangent.z;
			iss >> tangent.w;
			tangents.push_back(tangent);
			continue;
		}
	}
	if (flag) {
		BuildMesh(vertexs, normals, texcoords, tangents, faces);
		faces.clear();
	}
	in.close();
}

void Model::AddMesh(Mesh * m)
{
	meshes.push_back(m);
}

void Model::RemoveMesh(int num)
{
	if (num >= meshes.size())
		return;
	if (meshes[num])
		delete meshes[num];
	meshes.erase(meshes.begin() + num);
}

Model * Model::CreateFloor(const glm::vec3 & center, float radius)
{
	glm::vec3 leftTop = glm::vec3(center.x - radius, center.y, center.z - radius);
	glm::vec3 leftBottom = glm::vec3(center.x - radius, center.y, center.z + radius);
	glm::vec3 rightBottom = glm::vec3(center.x + radius, center.y, center.z + radius);
	glm::vec3 rightTop = glm::vec3(center.x + radius, center.y, center.z - radius);
	Mesh * p = Mesh::CreatePlane(leftTop, leftBottom, rightBottom, rightTop, glm::vec3(0, 1, 0));
	return new Model(p);
}

Model * Model::CreateSkyBox()
{
	Model * result = new Model(Mesh::CreateBox(glm::vec3(0.0f), 1.0f));
	//no shadow
	result->meshes[0]->material->SetShader(nullptr, 0);
	result->meshes[0]->material->SetShader(SkyBoxShader::GetInstance(), 1);
	return result;
}

void Model::BuildMesh(const std::vector<glm::vec3>& vertex, const std::vector<glm::vec3>& normal, const std::vector<glm::vec2>& texcoord, const std::vector<glm::vec4>& tangent, const std::vector<glm::mat3>& face)
{
	int faceNum = face.size();
	int vNum = 3 * faceNum;
	Mesh * o = new Mesh();
	int current = meshes.size();
	o->name = "object" + std::to_string(current + 1);
	meshes.push_back(o);
	bool calcTangent = tangent.empty();

	for (int i = 0; i < faceNum; i++) {
		Vertex v1(
			vertex[face[i][0].x - 1],
			glm::vec4(1.0f),
			texcoord[face[i][0].y - 1],
			normal[face[i][0].z - 1]
		);
		Vertex v2(
			vertex[face[i][1].x - 1],
			glm::vec4(1.0f),
			texcoord[face[i][1].y - 1],
			normal[face[i][1].z - 1]
		);
		Vertex v3(
			vertex[face[i][2].x - 1],
			glm::vec4(1.0f),
			texcoord[face[i][2].y - 1],
			normal[face[i][2].z - 1]
		);
		if (!calcTangent) {
			v1.tangent = tangent[face[i][0].x - 1];
			v2.tangent = tangent[face[i][1].y - 1];
			v3.tangent = tangent[face[i][2].z - 1];
		}
		else {
			//v1.tangent = v2.tangent = v3.tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			v1.tangent = Vertex::CalcTangent(v1, v2, v3);
			v2.tangent = v1.tangent;
			v3.tangent = v1.tangent;
		}
		o->AddTriangle(v1, v2, v3);
	}
}
