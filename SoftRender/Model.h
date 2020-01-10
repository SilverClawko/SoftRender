#ifndef MODEL_H
#define MODEL_H

#include "Object.h"
#include "Globel.h"


class Model
{

public:
	std::vector<Object> objects;
	Model() = default;

	~Model() = default;
	Model(const std::string &path) {
		LoadObj(path);
		//loadModel(path);
	}
	Model(const Model & model) {
		objects = model.objects;
	}
	Model & operator=(const Model & model) {
		if (&model == this)
			return *this;
		objects = model.objects;
		return *this;
	}
	void SetMaterial(const int &id,const Material& m) {
		objects[id].material = m;
	}

	void LoadObj(const std::string &path) {
		std::ifstream in(path);
		if (!in) {
			std::cout << "Open Obj File Error !" << std::endl;
			return;
		}

		std::vector<glm::vec3> vertexs;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texcoords;

		std::string line;

		int currentObjectNums = -1;
		bool flag = false;
		while (!in.eof()) {
			std::getline(in,line);
			if (!line.compare(0, 2, "v "))
			{
				if (!flag) {
					currentObjectNums++;
					Object o;
					objects.push_back(o);
					flag = true;
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
				texcoords.push_back(glm::vec2(vt.x,vt.y));
				continue;
			}
			if (!line.compare(0, 2, "f "))
			{
				if (flag)
					flag = false;
				line = line.substr(2);
				std::istringstream iss(line);
				char bar;
				int vIndex, vtIndex, vnIndex;
				// 1/1/1
				int offset = objects[currentObjectNums].mesh.VBO.size();
				for (int i = 0; i < 3; i++) {
					iss >> vIndex >> bar >> vtIndex >> bar >> vnIndex;
					Vertex vertex(vertexs[vIndex-1], glm::vec4(1, 1, 1, 1), texcoords[vtIndex-1], normals[vnIndex - 1]);
					objects[currentObjectNums].mesh.VBO.push_back(vertex);
					objects[currentObjectNums].mesh.EBO.push_back(offset + i);
				}
				//计算切线
				glm::vec3 pos1 = objects[currentObjectNums].mesh.VBO[offset].position;
				glm::vec3 pos2 = objects[currentObjectNums].mesh.VBO[offset+1].position;
				glm::vec3 pos3 = objects[currentObjectNums].mesh.VBO[offset+2].position;
				glm::vec2 uv1 = objects[currentObjectNums].mesh.VBO[offset].texcoord;
				glm::vec2 uv2 = objects[currentObjectNums].mesh.VBO[offset+1].texcoord;
				glm::vec2 uv3 = objects[currentObjectNums].mesh.VBO[offset+2].texcoord;
				glm::vec3 edge1 = pos2 - pos1;
				glm::vec3 edge2 = pos3 - pos1;
				glm::vec2 deltaUV1 = uv2 - uv1;
				glm::vec2 deltaUV2 = uv3 - uv1;

				float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

				glm::vec3 tangent;
				tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
				tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
				tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
				tangent = glm::normalize(tangent);
				objects[currentObjectNums].mesh.VBO[offset].tangent = tangent;
				objects[currentObjectNums].mesh.VBO[offset + 1].tangent = tangent;
				objects[currentObjectNums].mesh.VBO[offset + 2].tangent = tangent;

				if (iss >> vIndex) {
					iss >> bar >> vtIndex >> bar >> vnIndex;
					Vertex vertex(vertexs[vIndex - 1], glm::vec4(1, 1, 1, 1), texcoords[vtIndex - 1], normals[vnIndex - 1]);
					objects[currentObjectNums].mesh.VBO.push_back(vertex);
					objects[currentObjectNums].mesh.EBO.push_back(offset);
					objects[currentObjectNums].mesh.EBO.push_back(offset + 2);
					objects[currentObjectNums].mesh.EBO.push_back(offset + 3);	
					pos1 = objects[currentObjectNums].mesh.VBO[offset].position;
					pos2 = objects[currentObjectNums].mesh.VBO[offset + 2].position;
					pos3 = objects[currentObjectNums].mesh.VBO[offset + 3].position;
					uv1 = objects[currentObjectNums].mesh.VBO[offset].texcoord;
					uv2 = objects[currentObjectNums].mesh.VBO[offset + 2].texcoord;
					uv3 = objects[currentObjectNums].mesh.VBO[offset + 3].texcoord;
					edge1 = pos2 - pos1;
					edge2 = pos3 - pos1;
					deltaUV1 = uv2 - uv1;
					deltaUV2 = uv3 - uv1;

					f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

					tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
					tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
					tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
					tangent = glm::normalize(tangent);
					objects[currentObjectNums].mesh.VBO[offset].tangent = tangent;
					objects[currentObjectNums].mesh.VBO[offset + 2].tangent = tangent;
					objects[currentObjectNums].mesh.VBO[offset + 3].tangent = tangent;
				}
				continue;
			}


		}
		in.close();
	}

};

#endif