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
		while (!in.eof()) {
			std::getline(in,line);
			if (!line.compare(0, 15,"# object Object")) {
				
				std::string tmp = line.substr(15);
				currentObjectNums = atoi(tmp.c_str()) - 1;
				Object o;
				objects.push_back(o);
				continue;
			}
			if (!line.compare(0, 2, "v "))
			{
				line = line.substr(3);
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
				//¶þÎ¬ÎÆÀí z=0
				iss >> vt.z;
				texcoords.push_back(glm::vec2(vt.x,vt.y));
				continue;
			}
			if (!line.compare(0, 2, "f "))
			{
				line = line.substr(2);
				std::istringstream iss(line);
				char bar;
				int vIndex, vtIndex, vnIndex;
				// 1/1/1
				int offset = objects[currentObjectNums].mesh.VBO.size();
				for (int i = 0; i < 3; i++) {
					iss >> vIndex >> bar >> vtIndex >> bar >> vnIndex;
					if (vIndex > vertexs.size() || vtIndex > texcoords.size() || vnIndex > normals.size()) {
						int sb = 2;
						sb++;
					}
					Vertex vertex(vertexs[vIndex-1], glm::vec4(1, 1, 1, 1), texcoords[vtIndex-1], normals[vnIndex - 1]);
					objects[currentObjectNums].mesh.VBO.push_back(vertex);
					objects[currentObjectNums].mesh.EBO.push_back(offset + i);
				}
				continue;
			}


		}
		in.close();
	}

};





#endif